#pragma once

#include "includes.h"
#include "pure.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct { unsigned char MAKE_PAD(offset); type name; }

#define TORAD(x) ((x) * 0.01745329252)
#define W2S(x,y) sdk->world_to_screen(x, y)

class Ent {
public:
	union {
		DEFINE_MEMBER_N(int, client_id, 0x64);
		DEFINE_MEMBER_N(bool, m_bDormant, offsets::m_bDormant);
		DEFINE_MEMBER_N(int, m_iHealth, offsets::m_iHealth);
		DEFINE_MEMBER_N(int, m_iTeamNum, offsets::m_iTeamNum);
		DEFINE_MEMBER_N(Vec3, m_vecOrigin, offsets::m_vecOrigin);
		DEFINE_MEMBER_N(float, m_angEyeAnglesY, offsets::m_angEyeAnglesY);
		DEFINE_MEMBER_N(int, m_dwBoneMatrix, offsets::m_dwBoneMatrix);
		DEFINE_MEMBER_N(bool, m_bHasHelmet, offsets::m_bHasHelmet);
		DEFINE_MEMBER_N(float, m_flFlashDuration, offsets::m_flFlashDuration);
		DEFINE_MEMBER_N(bool, m_bInReload, offsets::m_bInReload);
		DEFINE_MEMBER_N(bool, is_c4_owner, offsets::is_c4_owner);
	};
};

class SDK {
public:
	float view_matrix[16];
	uintptr_t engine;
	uintptr_t client;
	ent_list::IClientEntityList* entity_list;
	ID3DXLine* LineL;
	ID3DXFont* FontF;
	void init();
	void update();
	uintptr_t get_local_player();
	bool is_valid_ent(Ent* ent);
	Vec3 get_bone_pos(Ent* ent, int bone);
	bool world_to_screen(Vec3 coords_3d, Vec2& coords_2d);
	Vec2 world_to_screen(Vec3 coords_3d);
	Vec3 transform_vec(Vec3 src, Vec3 ang, float d);
	Rect headbox_2D(Vec2 top, Vec2 bottom);
	HyperRect headbox_3D(Ent* ent);
	~SDK();
	struct {
		// Hacks.
		bool trigger_bot = false;
		bool bone_esp = false;
		bool skin_changer = false;
		bool bunny_hop = false;
		bool head_box_esp_2d = false;
		bool head_box_esp_3d = false;
		bool trigger_hs_only = true;
		bool bunny_hop_n6 = true;
		// Deprecated.
		bool show_boneIDs = false;
		bool show_textstatus = false;
	} config;
	struct {
		// Hacks.
		bool trigger_bot = false;
		bool bunny_hop = false;
		bool bone_esp = false;
		bool skin_changer = false;
		// Others.
		bool head_box_esp_2d = false;
		bool head_box_esp_3d = false;
	} hacks;
	struct {
		bool trigger_headshot_only = true;
		bool trigger_ignore_teamates = true;
		bool bone_esp_ignore_teamates = true;
		bool bunny_toggle_key = true;
		bool bunny_knife_only = true;
	} hack_settings;
	struct {
		// General colors.
		D3DCOLOR color_red = D3DCOLOR_ARGB(255, 255, 51, 78);
		D3DCOLOR color_orange = D3DCOLOR_ARGB(255, 255, 177, 51);
		D3DCOLOR color_green = D3DCOLOR_ARGB(255, 82, 224, 134);
		D3DCOLOR color_blue = D3DCOLOR_ARGB(255, 51, 180, 255);
		D3DCOLOR color_purple = D3DCOLOR_ARGB(255, 135, 98, 208);
		D3DCOLOR color_white = D3DCOLOR_ARGB(255, 255, 255, 255);
		// Menu theme.
		D3DCOLOR menu_background = D3DCOLOR_ARGB(255, 29, 32, 33);
		D3DCOLOR menu_skin = D3DCOLOR_ARGB(255, 235, 219, 178);
		D3DCOLOR menu_yellow = D3DCOLOR_ARGB(255, 250, 189, 47);
		D3DCOLOR menu_orange = D3DCOLOR_ARGB(255, 215, 153, 33);
		D3DCOLOR menu_red = D3DCOLOR_ARGB(255, 251, 73, 52);
		D3DCOLOR menu_green = D3DCOLOR_ARGB(255, 152, 151, 26);
		D3DCOLOR menu_white = D3DCOLOR_ARGB(255, 228, 228, 228);
		D3DCOLOR menu_gray_orange = D3DCOLOR_ARGB(255, 168, 153, 132);
		// Transparent colors.
		D3DCOLOR color_white_trans = D3DCOLOR_ARGB(100, 255, 255, 255);
		D3DCOLOR color_blue_trans = D3DCOLOR_ARGB(70, 51, 180, 255);
		D3DCOLOR color_red_trans = D3DCOLOR_ARGB(100, 255, 51, 78);
		D3DCOLOR color_purple_trans = D3DCOLOR_ARGB(100, 135, 98, 208);
		D3DCOLOR color_green_trans = D3DCOLOR_ARGB(70, 82, 224, 134);
		D3DCOLOR color_orange_trans = D3DCOLOR_ARGB(50, 255, 177, 51);
		// Transparent strengh.
		D3DCOLOR color_trans = D3DCOLOR_ARGB(0, 0, 0, 0);
		D3DCOLOR color_bones = D3DCOLOR_ARGB(100, 255, 255, 255);
		// Team colors.
		D3DCOLOR color_t = color_orange_trans;
		D3DCOLOR color_ct = color_purple_trans;
	} colors;
};