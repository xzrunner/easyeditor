#ifndef _EASYPARTICLE3D_LANGUAGE_H_
#define _EASYPARTICLE3D_LANGUAGE_H_

namespace eparticle3d
{

enum {
	LK_CENTER = 0,
	LK_OFFSET,
	LK_MIN,
	LK_MAX,
	LK_RANGE,
	LK_SPEED,
	LK_START,
	LK_END,

	LK_NAME,
	LK_PACKAGE,
	LK_COUNT,
	LK_LAYER,
	LK_EMISSION_TIME,
	LK_LIFE,
	LK_HORI,
	LK_VERT,
	LK_LINEAR_SPEED,
	LK_ANGULAR_SPEED,
	LK_DISTURBANCE,
	LK_GRAVITY,
	LK_LINEAR_ACC,
	LK_INERTIA,
	LK_FADEOUT_TIME,
	LK_BOUNCE,
	LK_ADDITIVE_BLAND,
	LK_START_RADIUS,
	LK_ORIENT_MOVEMENT,
	LK_ORIENT_PARENT,

	LK_REMOVE,
	LK_REMOVE_ALL,
	LK_COMPONENTS,

	LK_SCALE,
	LK_ROTATE,
	LK_COL_MUL,
	LK_COL_ADD,
	LK_ALPHA,
	LK_BIND_PS,
};

static const char* EN[] = {
	"center",
	"offset",
	"min",
	"max",
	"range (pixel)",
	"speed (pixel)",
	"start",
	"end",

	"Name",
	"Package",	
	"Count",
	"Layer",
	"Emission Time (ms)",
	"Life (ms)",
	"Horizontal (deg)",
	"Vertical (deg)",
	"Speed (pixel)",
	"Angular Speed (degree)",
	"Disturbance",
	"Gravity (pixel)",
	"Linear Acc (pixel)",
	"Inertia",
	"Fadeout Time (ms)",
	"Bounce",
	"Additive Blend",
	"Start Radius (pixel)",
	"Orient to Movement",
	"Orient to Parent",

	"Remove",
	"Remove All",
	"Components",

	"Scale (%)",
	"Rotate (deg)",
	"Col Mul",
	"Col Add",
	"Alpha",
	"Bind PS",
};

static const char* CN[] = {
	"中心",
	"偏移",
	"最小",
	"最大",
	"幅度",
	"速度",
	"起始",
	"结束",

	"名字",
	"包名",
	"数量",
	"层",
	"发射周期(毫秒)",
	"生命周期(毫秒)",
	"水平发射角度",
	"竖直发射角度",
	"线速度",
	"角速度",
	"扰动",
	"重力",
	"线性加速度",
	"转动惯量",
	"消逝时间",
	"弹跳",
	"叠加渲染",
	"起始半径",
	"朝向运动方向",
	"朝向父节点方向",

	"删除",
	"删除全部",
	"组件",

	"缩放(%)",
	"旋转",
	"乘色",
	"加色",
	"透明度",
	"绑定发射器",
};

static const char** LANG = CN;

}

#endif // _EASYPARTICLE3D_LANGUAGE_H_