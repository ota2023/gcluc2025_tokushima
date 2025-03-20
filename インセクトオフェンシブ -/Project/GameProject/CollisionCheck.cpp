#include "CollisionCheck.h"

bool IsHitBox3D(float _cx, float _cy, float _cz, float _dx, float _dy, float _dz, SRect3D _srcRect, SRect3D _destRect) {
    float al = _cx + _srcRect.TopX; //左上端(基点)の座標に
    float at = _cy + _srcRect.TopY; //上端
    float af = _cz + _srcRect.TopZ; // Zの開始位置
    float ar = al + _srcRect.Width; //右下端
    float ab = at + _srcRect.Height;//下端
    float aback = af + _srcRect.Depth; // Zの終了位置

    float bl = _dx + _destRect.TopX;
    float bt = _dy + _destRect.TopY;
    float bf = _dz + _destRect.TopZ; // Zの開始位置
    float br = bl + _destRect.Width;
    float bb = bt + _destRect.Height;
    float bback = bf + _destRect.Depth; // Zの終了位置

    if (al < br && bl < ar &&   // X軸の範囲が重なっている
        at < bb && bt < ab &&       // Y軸の範囲が重なっている
        af < bback && bf < aback) {  // Z軸の範囲が重なっている
        return true;
    }
    else {
        return false;
    }
}
