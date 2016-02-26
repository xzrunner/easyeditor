# EasyEditor

Game editor.

## Levels:
1. easyeditor: an editor framework, can be used to create 3 or 4.
2. symbol editor library: library for certain symbol, can be included by 3 or 4.
3. symbol editor: editor to create certain symbol, such as scale9, anim, complex, particle, and so on.
4. world editor: game scene editor, use 3's symbols to create game world.

### level 1 (easyeditor)

Modules|Desc
:-----:|:----
libdata | data
libedit | edit op, edit view
libgui  | wx wrapper
libmath | math
libmsg	| control
libphysics | box2d wrapper
librender | render
libview | view
libtool | tool

### level 2 (symbol library)
Modules|Symbol Desc|Stat
:-----:|:---------:|:---
libanim | 2d animatioin | 
libanim3d | 3d animation | todo
libanis | 2d anim's template |
libcomplex | compose other symbols |
libicon | texture node symbol for texture animation |
libimage | picture processing |
libmesh | mesh node symbol for mesh animation |
libmodeling | box2d | todo
libparticle2d | particle 2d model |
libparticle3d | particle 3d model |
libp3dinv | reverse particle 3d |
librespacker | symbol packer | should move to other level
libscale9 | scale9 for ui stretch |　
libshadow | shadow |
libshape | point, line, face and style |
libsketch | 3d model | todo
libterrain2d | 2d terrain: river, ocean |
libterrain3d | 3d terrain | todo
libtext | text | 
libtexture | texture tile |
easyejoy2d | ejoy2d sprite | todo

### level 3 (symbol editor)
Modules|Create Symbol|Stat
:-----:|:-----------:|:---
easyanim3d | libanim3d | todo
easyanimation | libanim |
easycomplex | libcomplex |
easyicon | libicon |
easyimage | image |
easymesh | libmesh |
easymmodeling | libmodeling | todo
easyp3dinv | libp3dinv |
easyparticle2d | libparticle2d |
easyparticle3d | libparticle3d |
easyscale9 | libscale9 |
easyshadow | libshadow | 
easyshape | libshape |
easysketch | libsketch | todo
easyterrain2d | libterrain2d |
easyterrain3d | libterrain3d |
easytexpacker | texture packer |
easyui | ui interface editor | should move to other level

### level 4 (world editor)

Modules|Desc
:-----:|:----
sg | 
lr |
