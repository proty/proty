from sphinx import addnodes
from sphinx.domains import Domain, ObjType, Index
from sphinx.locale import l_, _
from sphinx.directives import ObjectDescription
from sphinx.roles import XRefRole
from sphinx.domains.python import _pseudo_parse_arglist
from sphinx.util.nodes import make_refnode
from sphinx.util.docfields import Field, GroupedField, TypedField
from sphinx.util.compat import Directive

class ProtyObject(ObjectDescription):
    has_arguments = False
    display_prefix = None

    def handle_signature(self, sig, signode):
        sig = sig.strip()
        if '(' in sig and sig[-1:] == ')':
            prefix, arglist = sig.split('(', 1)
            arglist = arglist[:-1].strip()
        else:
            prefix = sig
            arglist = None
        if '.' in prefix:
            nameprefix, name = prefix.rsplit('.', 1)
        else:
            nameprefix = None
            name = prefix

        objectname = self.env.temp_data.get('pr:object')
        if nameprefix:
            if objectname:
                nameprefix = objectname + '.' + nameprefix
            fullname = nameprefix + '.' + name
        elif objectname:
            fullname = objectname + '.' + name
        else:
            objectname = ''
            fullname = name

        signode['object'] = objectname
        signode['fullname'] = fullname

        if self.display_prefix:
            signode += addnodes.desc_annotation(self.display_prefix,
                                                self.display_prefix)

        if nameprefix:
            signode += addnodes.desc_addname(nameprefix + '.', nameprefix + '.')
        signode += addnodes.desc_name(name, name)
        if self.has_arguments:
            if not arglist:
                signode += addnodes.desc_parameterlist()
            else:
                _pseudo_parse_arglist(signode, arglist)
        return fullname, nameprefix

    def add_target_and_index(self, name_obj, sig, signode):
        objectname = self.options.get('object', self.env.temp_data.get('pr:object'))
        fullname = name_obj[0]
        if fullname not in self.state.document.ids:
            signode['names'].append(fullname)
            signode['ids'].append(fullname)
            signode['first'] = not self.names
            self.state.document.note_explicit_target(signode)
            objects = self.env.domaindata['pr']['objects']
            if fullname in objects:
                self.env.warn(
                    self.env.docname,
                    'duplicate object description of %s, ' % fullname +
                    'other instance in ' +
                    self.env.doc2path(objects[fullname][0]),
                    self.lineno)
                objects[fullname] = self.env.docname, self.objtype

            indextext = self.get_index_text(objectname, name_obj)
            if indextext:
                self.indexnode['entries'].append(('single', indextext, fullname, ''))

    def get_index_text(self, objectname, name_obj):
        name, obj = name_obj
        if obj:
            name = name.split(".")[1]
        if self.objtype == 'function':
            if not obj:
                return _('%s() (built-in function)') % name
            return _('%s() (function in %s)') % (name, obj)
        elif self.objtype == 'slot':
            return _('%s (object in %s)') % (name, obj)
        return ''

class ProtyCallable(ProtyObject):
    has_arguments = True

    doc_field_types = [
        TypedField('parameter', label=l_('Parameters'),
                  names=('argument', 'arg', 'parameter', 'param'),
                  typerolename='func', typenames=('paramtype', 'type'),
                  can_collapse=True),
        GroupedField('exceptions', label=l_('Raises'), rolename='exc',
                     names=('raises', 'raise', 'exception', 'except'),
                     can_collapse=True),
        Field('returnvalue', label=l_('Returns'), has_arg=False,
              names=('returns', 'return')),
     ]

class ProtyXRefRole(XRefRole):
    def process_link(self, env, refnode, has_explicit_title, title, target):
        refnode['pr:object'] = env.temp_data.get('pr:object')
        if not has_explicit_title:
            title = title.lstrip('.')
            target = target.lstrip('~')
            if title[0:1] == '~':
                title = title[1:]
                dot = title.rfind('.')
                if dot != -1:
                    title = title[dot+1:]
        if target[0:1] == '.':
            target = target[1:]
            refnode['refspecific'] = True
        return title, target


class ProtyDomain(Domain):
    name= 'pr'
    label = 'Proty'
    object_types = {
        'function':  ObjType(l_('function'),  'func'),
        'exception': ObjType(l_('exception'), 'exc'),
        'prototype': ObjType(l_('prototype'), 'proto'),
        'slot':      ObjType(l_('slot'),      'slot'),
    }
    directives = {
        'function':  ProtyCallable,
        'exception': ProtyObject,
        'prototype': ProtyObject,
        'slot':      ProtyObject,
    }
    roles = {
        'func':  ProtyXRefRole(fix_parens=True),
        'exc':   ProtyXRefRole(),
        'proto': ProtyXRefRole(),
        'attr':  ProtyXRefRole(),
        'mod':   ProtyXRefRole(),
    }
    initial_data = {
        'objects': {},
    }

    def clear_doc(self, docname):
        for fullname, (fn, _) in self.data['objects'].items():
            if fn == docname:
                del self.data['objects'][fullname]

    def resolve_xref(self, env, fromdocname, builder, typ, target,
                     node, contnode):

        if target not in self.data['objects']:
            return None
        obj = self.data['objects']
        return make_refnode(builder, fromdocname, obj[0], target,
                            contnode, target)

    def get_objects(self):
        for refname, (docname, type) in self.data['objects'].items():
            yield(refname, refname, type, docname, refname, 1)

def setup(app):
    app.add_domain(ProtyDomain)
