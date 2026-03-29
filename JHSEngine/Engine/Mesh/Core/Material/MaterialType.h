#pragma once

enum EMaterialType
{
    //局部光照模型
    Lambertain = 0,
    HalfLambertain,
    Phong,
    BlinnPhong,
    Wrap,
    Minnaaer,
    Banded,
    Banded1,
    Banded2,
    Back,
    AnisotropyKajiyaKay,
    OrenNayar,
    
    //材质属性
    BaseColor,
    
    
    PBR = 20,
    Fresnel = 100,
    Max
};

enum EMaterialDisplayStatusType
{
    DefaultDisplay,         //默认效果
    PointDisplay,           //显示点
    WireframeDisplay,       //显示线框
    TriangleDisplay = 4,    //三角形显示
};