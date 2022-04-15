#pragma once

#include "includes.h"

struct TextAlignFormat {
	DWORD center = DT_CENTER;
	DWORD left = DT_LEFT;
	DWORD right = DT_RIGHT;
};

void DrawFilledRect(int x, int y, int width, int height, D3DCOLOR color);

void DrawLine(int src_x, int src_y, int dst_x, int dst_y, int thickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);

void DrawBoneEsp(Ent* ent);
void DrawBoneIDs(Ent* ent);

void DrawHeadBox2D(const Vec2& ent_head, Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color);
void DrawHeadBox3D(Vec3 top, Vec3 bottom, float angle, int width, int thickness, D3DCOLOR color);

void DrawMenuText(const char* text, float x, float y, int height, D3DCOLOR color, DWORD format);
void DrawText(const char* text, float x, float y, D3DCOLOR color, bool contrast = false);
void DrawText(const char* prefix, int text, float x, float y, D3DCOLOR color, bool contrast = false);
void DrawText(const char* prefix, float text, float x, float y, D3DCOLOR color, bool contrast = false);

void DrawText(const char* text, float x, float y, D3DCOLOR color, bool contrast, DWORD format);
void DrawText(const char* text, float x, float y, D3DCOLOR color, bool contrast, DWORD format, int text_num);
void DrawText(const char* text, float x, float y, D3DCOLOR color, bool contrast, DWORD format, float text_num);