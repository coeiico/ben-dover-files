#include "canvas.h"

extern SDK* sdk;

void DrawFilledRect(int x, int y, int width, int height, D3DCOLOR color) {
	D3DRECT rect = {
		x,
		y,
		x + width,
		y + height
	};
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int src_x, int src_y, int dst_x, int dst_y, int thickness, D3DCOLOR color) {
	if (!sdk->LineL) D3DXCreateLine(pDevice, &sdk->LineL);
	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(src_x, src_y);
	Line[1] = D3DXVECTOR2(dst_x, dst_y);
	sdk->LineL->SetWidth(thickness);
	sdk->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawBoneLine(Ent* ent, int from_bone, int to_bone) {
	D3DCOLOR color = sdk->colors.color_bones;
	// if (ent->is_c4_owner) color = D3DCOLOR_ARGB(255, 135, 98, 208);
	if (ent->m_flFlashDuration > 0) color = sdk->colors.color_white;
	Vec3 from_bone_pos_3d = sdk->get_bone_pos(ent, from_bone);
	Vec3 to_bone_pos_3d = sdk->get_bone_pos(ent, to_bone);
	Vec2 from_bone_pos_2d, to_bone_pos_2d;
	if (W2S(from_bone_pos_3d, from_bone_pos_2d) && W2S(to_bone_pos_3d, to_bone_pos_2d)) {
		//DrawLine(from_bone_pos_2d.x, from_bone_pos_2d.y, to_bone_pos_2d.x, to_bone_pos_2d.y, 2, color);
		DrawLine(from_bone_pos_2d, to_bone_pos_2d, 2, color);
	}
}

void DrawBoneIDs(Ent* ent) {
	for (int i = 0; i < 128; i++) {
		Vec2 bone_pos_2d;
		if (W2S(sdk->get_bone_pos(ent, i), bone_pos_2d)) {
			DrawText(std::to_string(i).c_str(), bone_pos_2d.x, bone_pos_2d.y, sdk->colors.color_white);
		}
	}
}

void DrawBoneEsp(Ent* ent) {
	//char name[128];
	//strcpy_s(name, (char*) (*(uintptr_t*) ((uintptr_t) ent + 0x6C) + 0x04));
	//char* model_name = &name[35];
	char* model_name = &((char*) (*(uintptr_t*) ((uintptr_t) ent + 0x6c) + 0x4))[35];

	/********** Special Models **********/
	if (strstr(model_name, "ctm_st6_variante")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 0, 69);
		DrawBoneLine(ent, 69, 70);
		DrawBoneLine(ent, 0, 76);
		DrawBoneLine(ent, 76, 77);
	}
	/************************************/

	else if (strstr(model_name, "ctm_idf")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 7, 41);
		DrawBoneLine(ent, 41, 42);
		DrawBoneLine(ent, 42, 43);
		DrawBoneLine(ent, 0, 71);
		DrawBoneLine(ent, 71, 72);
		DrawBoneLine(ent, 0, 78);
		DrawBoneLine(ent, 78, 79);
	} else if (strstr(model_name, "tm_leet")) {
		sdk->colors.color_bones = sdk->colors.color_t;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 7, 39);
		DrawBoneLine(ent, 39, 65);
		DrawBoneLine(ent, 65, 41);
		DrawBoneLine(ent, 0, 79);
		DrawBoneLine(ent, 79, 75);
		DrawBoneLine(ent, 0, 72);
		DrawBoneLine(ent, 72, 68);
	} else if (strstr(model_name, "ctm_sas")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 40);
		DrawBoneLine(ent, 40, 41);
		DrawBoneLine(ent, 41, 42);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 0, 82);
		DrawBoneLine(ent, 82, 83);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
	} else if (strstr(model_name, "tm_separatist")) {
		sdk->colors.color_bones = sdk->colors.color_t;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
		DrawBoneLine(ent, 0, 66);
		DrawBoneLine(ent, 66, 67);
	} else if (strstr(model_name, "tm_professional")) {
		sdk->colors.color_bones = sdk->colors.color_t;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 40, 41);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 0, 78);
		DrawBoneLine(ent, 78, 79);
		DrawBoneLine(ent, 0, 71);
		DrawBoneLine(ent, 71, 72);
	} else if (strstr(model_name, "ctm_fbi")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 40, 41);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 0, 72);
		DrawBoneLine(ent, 72, 73);
		DrawBoneLine(ent, 0, 81);
		DrawBoneLine(ent, 81, 82);
	} else if (strstr(model_name, "ctm_st6")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
		DrawBoneLine(ent, 0, 66);
		DrawBoneLine(ent, 66, 67);
	} else if (strstr(model_name, "ctm_swat")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 13, 14);
		DrawBoneLine(ent, 7, 40);
		DrawBoneLine(ent, 40, 41);
		DrawBoneLine(ent, 41, 42);
		DrawBoneLine(ent, 0, 68);
		DrawBoneLine(ent, 68, 69);
		DrawBoneLine(ent, 0, 75);
		DrawBoneLine(ent, 75, 76);
	} else if (strstr(model_name, "tm_phoenix")) {
		sdk->colors.color_bones = sdk->colors.color_t;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
		DrawBoneLine(ent, 0, 66);
		DrawBoneLine(ent, 66, 67);
	} else if (strstr(model_name, "ctm_gign")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 12, 13);
		DrawBoneLine(ent, 7, 40);
		DrawBoneLine(ent, 40, 41);
		DrawBoneLine(ent, 41, 42);
		DrawBoneLine(ent, 0, 77);
		DrawBoneLine(ent, 77, 78);
		DrawBoneLine(ent, 0, 70);
		DrawBoneLine(ent, 70, 71);
	} else if (strstr(model_name, "tm_balkan")) {
		sdk->colors.color_bones = sdk->colors.color_t;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
		DrawBoneLine(ent, 0, 66);
		DrawBoneLine(ent, 66, 67);
	} else if (strstr(model_name, "ctm_gsg9")) {
		sdk->colors.color_bones = sdk->colors.color_ct;
		DrawBoneLine(ent, 8, 7);
		DrawBoneLine(ent, 7, 0);
		DrawBoneLine(ent, 7, 10);
		DrawBoneLine(ent, 10, 11);
		DrawBoneLine(ent, 11, 12);
		DrawBoneLine(ent, 7, 38);
		DrawBoneLine(ent, 38, 39);
		DrawBoneLine(ent, 39, 40);
		DrawBoneLine(ent, 0, 73);
		DrawBoneLine(ent, 73, 74);
		DrawBoneLine(ent, 0, 66);
		DrawBoneLine(ent, 66, 67);
	}
}

void DrawMenuText(const char* text, float x, float y, int height, D3DCOLOR color, DWORD format) {
	RECT rect;
	SetRect(&rect, x, y, x, y);

	//D3DXCreateFontW(
	//	LPDIRECT3DDEVICE9       pDevice,
	//	INT                     Height,
	//	UINT                    Width,
	//	UINT                    Weight,
	//	UINT                    MipLevels,
	//	BOOL                    Italic,
	//	DWORD                   CharSet,
	//	DWORD                   OutputPrecision,
	//	DWORD                   Quality,
	//	DWORD                   PitchAndFamily,
	//	LPCWSTR                 pFaceName,
	//	LPD3DXFONT * ppFont);

	if (!sdk->FontF)
		D3DXCreateFont(pDevice, height, 0,
		FW_MEDIUM, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		MONO_FONT | FF_DONTCARE, L"Fira Code", &sdk->FontF);
	//STDMETHOD_(INT, DrawTextA)(THIS_ LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color) PURE;
	sdk->FontF->DrawTextA(NULL, text, -1, &rect, format | DT_NOCLIP, color);
}

void DrawText(const char* text, float x, float y, D3DCOLOR color, bool contrast) {
	RECT rect;

	//D3DXCreateFontW(
	//	LPDIRECT3DDEVICE9       pDevice,
	//	INT                     Height,
	//	UINT                    Width,
	//	UINT                    Weight,
	//	UINT                    MipLevels,
	//	BOOL                    Italic,
	//	DWORD                   CharSet,
	//	DWORD                   OutputPrecision,
	//	DWORD                   Quality,
	//	DWORD                   PitchAndFamily,
	//	LPCWSTR                 pFaceName,
	//	LPD3DXFONT * ppFont);

	if (!sdk->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_MEDIUM, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, MONO_FONT | FF_DONTCARE, L"Fira Code", &sdk->FontF);
	if (contrast) {
		SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
		sdk->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	SetRect(&rect, x, y, x, y);
	sdk->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void DrawText(const char* prefix, int text, float x, float y, D3DCOLOR color, bool contrast) {
	std::stringstream ss_text;
	ss_text << text;
	std::string s_text = prefix + ss_text.str();
	char* msg_text = (char*) s_text.c_str();
	RECT rect;
	if (!sdk->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_MEDIUM, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, MONO_FONT | FF_DONTCARE, L"Fira Code", &sdk->FontF);
	if (contrast) {
		SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
		sdk->FontF->DrawTextA(NULL, msg_text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	SetRect(&rect, x, y, x, y);
	sdk->FontF->DrawTextA(NULL, msg_text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void DrawText(const char* prefix, float text, float x, float y, D3DCOLOR color, bool contrast) {
	std::stringstream ss_text;
	ss_text << text;
	std::string s_text = prefix + ss_text.str();
	char* msg_text = (char*) s_text.c_str();
	RECT rect;
	if (!sdk->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_MEDIUM, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, MONO_FONT | FF_DONTCARE, L"Fira Code", &sdk->FontF);
	if (contrast) {
		SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
		sdk->FontF->DrawTextA(NULL, msg_text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	SetRect(&rect, x, y, x, y);
	sdk->FontF->DrawTextA(NULL, msg_text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void DrawHeadBox2D(const Vec2& ent_head, Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color) {
	int height = static_cast<int>(std::abs(top.y - bottom.y));
	Vec2 top_left, top_right;
	Vec2 bottom_left, bottom_right;
	top_left.x = top.x - static_cast<float>(height);
	top_left.y = top.y;
	top_right.x = top.x + static_cast<float>(height);
	top_right.y = top.y;
	bottom_left.x = bottom.x - static_cast<float>(height);
	bottom_left.y = bottom.y;
	bottom_right.x = bottom.x + static_cast<float>(height);
	bottom_right.y = bottom.y;
	DrawLine(top_left, top_right, thickness, color);
	DrawLine(top_right, bottom_right, thickness, color);
	DrawLine(bottom_right, bottom_left, thickness, color);
	DrawLine(bottom_left, top_left, thickness, color);
}

void DrawHeadBox3D(Vec3 top, Vec3 bottom, float angle, int width, int thickness, D3DCOLOR color) {
	int height3D = std::abs(top.z - bottom.z);
	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.x = bottom.x + (cos(TORAD(angle + 45)) * width / 2);
	b1.y = bottom.y + (sin(TORAD(angle + 45)) * width / 2);
	b1.z = bottom.z;
	b2.x = bottom.x + (cos(TORAD(angle + 135)) * width / 2);
	b2.y = bottom.y + (sin(TORAD(angle + 135)) * width / 2);
	b2.z = bottom.z;
	b3.x = bottom.x + (cos(TORAD(angle + 225)) * width / 2);
	b3.y = bottom.y + (sin(TORAD(angle + 225)) * width / 2);
	b3.z = bottom.z;
	b4.x = bottom.x + (cos(TORAD(angle + 315)) * width / 2);
	b4.y = bottom.y + (sin(TORAD(angle + 315)) * width / 2);
	b4.z = bottom.z;
	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;
	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;
	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;
	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;
	Vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2)) {
		// Vertical lines.
		DrawLine(t1_2, b1_2, thickness, color);
		DrawLine(t2_2, b2_2, thickness, color);
		DrawLine(t3_2, b3_2, thickness, color);
		DrawLine(t4_2, b4_2, thickness, color);
		// Horizontal lines, lid.
		DrawLine(t1_2, t2_2, thickness, color);
		DrawLine(t2_2, t3_2, thickness, color);
		DrawLine(t3_2, t4_2, thickness, color);
		DrawLine(t4_2, t1_2, thickness, color);
		// Horizontal lines, base.
		DrawLine(b1_2, b2_2, thickness, color);
		DrawLine(b2_2, b3_2, thickness, color);
		DrawLine(b3_2, b4_2, thickness, color);
		DrawLine(b4_2, b1_2, thickness, color);
	}
}