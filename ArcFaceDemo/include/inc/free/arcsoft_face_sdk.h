/*******************************************************************************
* Copyright(c) ArcSoft, All right reserved.
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
* and confidential information.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
* AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy,
* distribute, modify, or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout
* thereof.
*********************************************************************************/

#ifndef _ARCSOFT_SDK_ASF_H_
#define _ARCSOFT_SDK_ASF_H_

#include "amcomdef.h"
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASF_NONE				0x00000000	//无属性
#define ASF_FACE_DETECT			0x00000001	//此处detect可以是tracking或者detection两个引擎之一，具体的选择由detect mode 确定
#define ASF_FACERECOGNITION		0x00000004	//人脸特征
#define ASF_AGE					0x00000008	//年龄
#define ASF_GENDER				0x00000010	//性别
#define ASF_FACE3DANGLE			0x00000020	//3D角度
#define ASF_LIVENESS			0x00000080	//RGB活体
#define ASF_IR_LIVENESS			0x00000400	//红外活体

#define ASF_DETECT_MODE_VIDEO 	0x00000000		//Video模式，一般用于多帧连续检测
#define ASF_DETECT_MODE_IMAGE 	0xFFFFFFFF		//Image模式，一般用于静态图的单次检测


//版本信息
typedef struct {
	MPChar Version;							// 版本号
	MPChar BuildDate;						// 构建日期
	MPChar CopyRight;						// Copyright
}ASF_VERSION;

//检测时候 Orientation 优先级别
typedef MInt32 ASF_OrientPriority;
enum ArcSoftFace_OrientPriority {
	ASF_OP_0_ONLY = 0x1,			// 0, 0, ...
	ASF_OP_90_ONLY = 0x2,			// 90, 90, ...
	ASF_OP_270_ONLY = 0x3,			// 270, 270, ...
	ASF_OP_180_ONLY = 0x4,			// 180, 180, ...
	ASF_OP_0_HIGHER_EXT = 0x5,		// 0, 90, 270, 180, 0, 90, 270, 180, ...
									// 当前SDK版本，VIDEO模式下支持ASF_OP_0_HIGHER_EXT检测，IMAGE模式不支持
};

//orientation 角度，逆时针方向	
typedef MInt32 ASF_OrientCode;
enum ArcSoftFace_OrientCode {
	ASF_OC_0 = 0x1,			// 0 degree 
	ASF_OC_90 = 0x2,		// 90 degree 
	ASF_OC_270 = 0x3,		// 270 degree 
	ASF_OC_180 = 0x4,   	// 180 degree 
	ASF_OC_30 = 0x5,		// 30 degree 
	ASF_OC_60 = 0x6,		// 60 degree 
	ASF_OC_120 = 0x7,		// 120 degree 
	ASF_OC_150 = 0x8,		// 150 degree 
	ASF_OC_210 = 0x9,		// 210 degree 
	ASF_OC_240 = 0xa,		// 240 degree 
	ASF_OC_300 = 0xb,		// 300 degree 
	ASF_OC_330 = 0xc		// 330 degree 
};

//单人脸信息
typedef struct {
	MRECT		faceRect;		// 人脸框信息
	MInt32		faceOrient;		// 输入图像的角度，可以参考 ArcFaceCompare_OrientCode
} ASF_SingleFaceInfo, *LPASF_SingleFaceInfo;

//多人脸信息
typedef struct {
	MRECT* 		 faceRect;			// 人脸框信息
	MInt32*		 faceOrient;		// 输入图像的角度，可以参考 ArcFaceCompare_OrientCode . 
	MInt32		 faceNum;			// 检测到的人脸个数
	MInt32*      faceID;			// face ID，IMAGE模式下不返回FaceID
}ASF_MultiFaceInfo, *LPASF_MultiFaceInfo;

typedef struct{
	MPChar startTime;		//开始时间
	MPChar endTime;			//截止时间
	MPChar platform;		//平台
	MPChar sdkType;			//sdk类型
	MPChar appId;			//APPID
	MPChar sdkKey;			//SDKKEY
	MPChar sdkVersion;		//SDK版本号
	MPChar fileVersion;		//激活文件版本号
}ASF_ActiveFileInfo, *LPASF_ActiveFileInfo;

/*******************************************************************************************
* 获取激活文件信息接口
*******************************************************************************************/
MRESULT ASFGetActiveFileInfo(
	LPASF_ActiveFileInfo  activeFileInfo  // [out] 激活文件信息
);

/*******************************************************************************************
* 激活接口
*******************************************************************************************/
MRESULT ASFOnlineActivation(
	MPChar				AppId,			// [in]  APPID	官网下载
	MPChar				SDKKey			// [in]  SDKKEY	官网下载
);

/*******************************************************************************************
* 激活接口，该接口与ASFOnlineActivation接口功能一致，推荐使用ASFOnlineActivation接口
*******************************************************************************************/
MRESULT ASFActivation(
	MPChar				AppId,			// [in]  APPID	官网下载
	MPChar				SDKKey			// [in]  SDKKEY	官网下载
);

/************************************************************************
* 初始化引擎
************************************************************************/
MRESULT ASFInitEngine(
	MUInt32				detectMode,					// [in]	AF_DETECT_MODE_VIDEO 视频模式：适用于摄像头预览，视频文件识别
													//		AF_DETECT_MODE_IMAGE 图片模式：适用于静态图片的识别
	ASF_OrientPriority	detectFaceOrientPriority,	// [in]	检测脸部的角度优先值，参考 ArcFaceCompare_OrientPriority
	MInt32				detectFaceScaleVal,			// [in] 用于数值化表示的最小人脸尺寸，该尺寸代表人脸尺寸相对于图片长边的占比
													// video 模式有效值范围[2, 32], 推荐值为 16
													// image 模式有效值范围[2, 32], 推荐值为 30
	MInt32				detectFaceMaxNum,			// [in] 最大需要检测的人脸个数
	MInt32				combinedMask,				// [in] 用户选择需要检测的功能组合，可单个或多个
	MHandle*			hEngine						// [out] 初始化返回的引擎handle
);

/******************************************************
* 人脸检测,目前不支持IR图像数据检测
******************************************************/
MRESULT ASFDetectFaces(
	MHandle				hEngine,				// [in] 引擎handle
	MInt32				width,					// [in] 图片宽度
	MInt32				height,					// [in] 图片高度
	MInt32				format,					// [in] 颜色空间格式
	MUInt8*				imgData,				// [in] 图片数据
	LPASF_MultiFaceInfo	detectedFaces			// [out]检测到的人脸信息 
);

typedef struct{
	MFloat		thresholdmodel_BGR;
	MFloat		thresholdmodel_IR;
}ASF_LivenessThreshold, *LPASF_LivenessThreshold;

/************************************************************************
* 取值范围[0-1]内部默认数值RGB-0.75，IR-0.7， 用户可以根据实际需求，设置不同的阈值
************************************************************************/
MRESULT ASFSetLivenessParam(
	MHandle					hEngine,		// [in] 引擎handle
	LPASF_LivenessThreshold threshold		// [in] 活体置信度
);
	
/************************************************************************
* 年龄/性别/3D角度（该接口仅支持RGB图像），最多支持4张人脸信息检测，超过部分返回未知
* RGB活体仅支持单人脸检测
* 该接口不支持检测IR活体
************************************************************************/
MRESULT ASFProcess(
	MHandle				hEngine,			// [in] 引擎handle
	MInt32				width,				// [in] 图片宽度
	MInt32				height,				// [in] 图片高度
	MInt32				format,				// [in] 颜色空间格式
	MUInt8*				imgData,			// [in] 图片数据
	LPASF_MultiFaceInfo	detectedFaces,		// [in] 人脸信息，用户根据待检测的功能选择需要使用的人脸。
	MInt32				combinedMask		// [in] 只支持初始化时候指定需要检测的功能，在process时进一步在这个已经指定的功能集中继续筛选
											//      例如初始化的时候指定检测年龄和性别，在process的时候可以只检测年龄，但是不能检测除年龄和性别之外的功能    
);

/************************************************************************
* 该接口目前仅支持单人脸IR活体检测（不支持年龄、性别、3D角度的检测），默认取第一张人脸
************************************************************************/
MRESULT ASFProcess_IR(
	MHandle				hEngine,			// [in] 引擎handle
	MInt32				width,				// [in] 图片宽度
	MInt32				height,				// [in] 图片高度
	MInt32				format,				// [in] 颜色空间格式
	MUInt8*				imgData,			// [in] 图片数据
	LPASF_MultiFaceInfo	detectedFaces,		// [in] 人脸信息，用户根据待检测的功能选择需要使用的人脸。
	MInt32				combinedMask		// [in] 目前只支持传入ASF_IR_LIVENESS属性的传入，且初始化接口需要传入 
);

/************************************************************************
* 销毁引擎
************************************************************************/
MRESULT ASFUninitEngine(
	MHandle hEngine
);

/************************************************************************
* 获取版本信息
************************************************************************/
const ASF_VERSION* ASFGetVersion(
	MHandle hEngine
);

//******************************** 人脸识别相关 *********************************************
typedef struct {
	MByte*		feature;		// 人脸特征信息
	MInt32		featureSize;	// 人脸特征信息长度	
}ASF_FaceFeature, *LPASF_FaceFeature;

/************************************************************************
* 单人脸特征提取
************************************************************************/
MRESULT ASFFaceFeatureExtract(
	MHandle					hEngine,		// [in]	引擎handle
	MInt32					width,			// [in] 图片宽度
	MInt32					height,			// [in] 图片高度
	MInt32					format,			// [in] 颜色空间格式
	MUInt8*					imgData,		// [in] 图片数据
	LPASF_SingleFaceInfo	faceInfo,		// [in] 单张人脸位置和角度信息
	LPASF_FaceFeature		feature			// [out] 人脸特征
);

/************************************************************************
* 人脸特征比对
************************************************************************/
MRESULT ASFFaceFeatureCompare(
	MHandle					hEngine,			// [in] 引擎handle
	LPASF_FaceFeature		feature1,			// [in] 待比较人脸特征1
	LPASF_FaceFeature		feature2,			// [in] 待比较人脸特征2
	MFloat*					confidenceLevel		// [out] 比较结果，置信度数值
);

//******************************** 年龄相关 **********************************************
typedef struct
{
	MInt32*	ageArray;				// "0" 代表不确定，大于0的数值代表检测出来的年龄结果
	MInt32	num;					// 检测的人脸个数
}ASF_AgeInfo, *LPASF_AgeInfo;

/************************************************************************
* 获取年龄信息
************************************************************************/
MRESULT ASFGetAge(
	MHandle hEngine,				// [in] 引擎handle
	LPASF_AgeInfo ageInfo			// [out] 检测到的年龄信息
);

//******************************** 性别相关 **********************************************
typedef struct
{
	MInt32*	genderArray;			//"0" 表示 男性, "1" 表示 女性, "-1" 表示不确定
	MInt32	num;					// 检测的人脸个数	
}ASF_GenderInfo, *LPASF_GenderInfo;

/************************************************************************
* 获取性别信息
************************************************************************/
MRESULT ASFGetGender(
	MHandle hEngine,				// [in] 引擎handle
	LPASF_GenderInfo genderInfo		// [out] 检测到的性别信息
);

//******************************** 人脸3D 角度信息 ***********************************
typedef struct
{
	MFloat* roll;
	MFloat* yaw;
	MFloat* pitch;
	MInt32* status;					//0: 正常，其他数值：出错
	MInt32 num;
}ASF_Face3DAngle, *LPASF_Face3DAngle;

/************************************************************************
* 获取3D角度信息
************************************************************************/
MRESULT ASFGetFace3DAngle(
	MHandle hEngine,					// [in] 引擎handle
	LPASF_Face3DAngle p3DAngleInfo		// [out] 检测到脸部3D 角度信息
);

//******************************** 活体信息 ***********************************
typedef struct
{
	MInt32*	isLive;			// [out] 判断是否真人， 0：非真人；
	//						1：真人；
	//						-1：不确定； 
	//						-2:传入人脸数>1；
	MInt32 num;
}ASF_LivenessInfo, *LPASF_LivenessInfo;

/************************************************************************
* 获取RGB活体结果
************************************************************************/
MRESULT ASFGetLivenessScore(
	MHandle hEngine,					// [in] 引擎handle
	LPASF_LivenessInfo livenessInfo		// [out] 检测RGB活体结果
);

/************************************************************************
* 获取IR活体结果
************************************************************************/
MRESULT ASFGetLivenessScore_IR(
	MHandle				hEngine,			// [in] 引擎handle
	LPASF_LivenessInfo	 irLivenessInfo		// [out] 检测到IR活体结果
);

#ifdef __cplusplus
}
#endif
#endif