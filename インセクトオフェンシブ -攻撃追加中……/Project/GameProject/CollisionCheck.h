#pragma once


//矩形用構造体
struct SRect3D {
    int TopX;   // 左上のX座標
    int TopY;   // 左上のY座標
    int TopZ;   // 前面のZ座標（奥行きを考慮）
    int Width;  // 矩形の幅（X方向）
    int Height; // 矩形の高さ（Y方向）
    int Depth;  // 矩形の奥行き（Z方向）
};

//矩形領域の当たり判定
bool IsHitBox3D(float _cx, float _cy, float _cz, 
                float _dx, float _dy, float _dz, 
                SRect3D _srcRect, SRect3D _destRect);