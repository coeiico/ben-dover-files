#include "info_overlay.h"

void Overlay::Display() {
	if (config.show_client_info) ShowClientInfo();
	if (config.show_ents_info) ShowEntsInfo();
}

void Overlay::ShowClientInfo() {
	if (config.client.is_show_crosshair_position) show_crosshair_position();
}

void Overlay::ShowEntsInfo() {
	Ent* ent;
	int above_ent_pos;
	int below_ent_pos;
	for (int i = 0; i < 32; i++) {
		above_ent_pos = 0;
		below_ent_pos = 0;
		ent = (Ent*) sdk->entity_list->GetClientEntity(i);
		if (!sdk->is_valid_ent(ent)) continue;
		// Bone IDs.
		if (config.ents.is_show_bone_ids) show_bone_ids(ent);
		// Head box esp 2D.
		if (config.ents.is_show_headbox_2d_coords) show_headbox_2d(ent);
		// Head box esp 3D.
		if (config.ents.is_show_headbox_3d_coords) show_headbox_3d(ent);
		// Player model name.
		if (config.ents.is_show_player_model_name) {
			show_player_model_name(ent, below_ent_pos);
			below_ent_pos += 1;
		}
		// Entity ID info.
		if (config.ents.is_show_entity_id_info) {
			show_entity_id_info(ent, below_ent_pos);
			below_ent_pos += 2;
		}
	}
}

void Overlay::show_bone_ids(Ent* ent) {
	for (int i = 0; i < 128; i++) {
		Vec2 screen;
		if (W2S(sdk->get_bone_pos(ent, i), screen)) {
			DrawText(std::to_string(i).c_str(), screen.x, screen.y, sdk->colors.color_white);
		}
	}
}

void Overlay::show_crosshair_position() {
	Vec3 punch_angle;
	Vec2 crosshair_position;
	punch_angle = *(Vec3*) (uintptr_t*) (sdk->get_local_player() + offsets::m_aimPunchAngle);
	crosshair_position.x = window_width / 2 - (window_width / 90 * punch_angle.y);
	crosshair_position.y = window_height / 2 + (window_height / 90 * punch_angle.x);
	DrawText("x: ",
		crosshair_position.x,
		(window_width / 2) + 24,
		(window_height / 2),
		sdk->colors.color_white
	);
	DrawText("y: ",
		crosshair_position.y,
		(window_width / 2) + 24,
		(window_height / 2) + 12,
		sdk->colors.color_white
	);
}

void Overlay::show_player_model_name(Ent* ent, int pos) {
	Vec2 ent_position;
	if (sdk->world_to_screen(ent->m_vecOrigin, ent_position)) {
		char* model_name = &((char*) (*(uintptr_t*) ((uintptr_t) ent + 0x6c) + 0x4))[35];
		DrawText(model_name, ent_position.x, ent_position.y + (pos * 12), sdk->colors.color_white_trans);
	}
}

void Overlay::show_entity_id_info(Ent* ent, int pos) {
	Vec2 ent_position;
	int crosshair_id = *(int*) (sdk->get_local_player() + offsets::m_iCrosshairId);
	if (sdk->world_to_screen(ent->m_vecOrigin, ent_position)) {
		DrawText("Entity ID: ", ent->client_id, ent_position.x, ent_position.y + (pos * 12), sdk->colors.color_blue);
		DrawText("Crosshair ID: ", crosshair_id, ent_position.x, ent_position.y + (++pos * 12), sdk->colors.color_blue);
	}
}

void Overlay::show_headbox_2d(Ent* ent) {
	Vec3 ent_head_3d = sdk->get_bone_pos(ent, 8); ent_head_3d.z += 8;
	Vec2 bottom;
	if (sdk->world_to_screen(ent_head_3d, bottom)) {
		Vec2 top;
		if (sdk->world_to_screen(sdk->get_bone_pos(ent, 7), top)) {
			Rect head_box_2d;
			head_box_2d = sdk->headbox_2D(top, bottom);
			D3DCOLOR color = sdk->colors.color_white;
			DrawText("tl x: ", head_box_2d.top_left.x, head_box_2d.top_left.x, head_box_2d.top_left.y, color);
			DrawText("tl y: ", head_box_2d.top_left.y, head_box_2d.top_left.x, head_box_2d.top_left.y + 12, color);
			DrawText("tr x: ", head_box_2d.top_right.x, head_box_2d.top_right.x, head_box_2d.top_right.y, color);
			DrawText("tr y: ", head_box_2d.top_right.y, head_box_2d.top_right.x, head_box_2d.top_right.y + 12, color);
			DrawText("bl x: ", head_box_2d.bottom_left.x, head_box_2d.bottom_left.x, head_box_2d.bottom_left.y, color);
			DrawText("bl y: ", head_box_2d.bottom_left.y, head_box_2d.bottom_left.x, head_box_2d.bottom_left.y + 12, color);
			DrawText("br x: ", head_box_2d.bottom_right.x, head_box_2d.bottom_right.x, head_box_2d.bottom_right.y, color);
			DrawText("br y: ", head_box_2d.bottom_right.y, head_box_2d.bottom_right.x, head_box_2d.bottom_right.y + 12, color);
		}
	}
}

void Overlay::show_headbox_3d(Ent* ent) {
	D3DCOLOR color = sdk->colors.color_white;
	HyperRect head_box_3d = sdk->headbox_3D(ent);
	// Bottom.
	DrawText("(Z) b1 x: ", head_box_3d.bottom.ax, head_box_3d.bottom.a.x, head_box_3d.bottom.a.y, color);
	DrawText("(Z) b2 x: ", head_box_3d.bottom.bx, head_box_3d.bottom.b.x, head_box_3d.bottom.b.y, color);
	DrawText("(Z) b3 x: ", head_box_3d.bottom.cx, head_box_3d.bottom.c.x, head_box_3d.bottom.c.y, color);
	DrawText("(Z) b4 x: ", head_box_3d.bottom.dx, head_box_3d.bottom.d.x, head_box_3d.bottom.d.y, color);
	DrawText("(X) b1 y: ", head_box_3d.bottom.ay, head_box_3d.bottom.a.x, head_box_3d.bottom.a.y + 12, color);
	DrawText("(X) b2 y: ", head_box_3d.bottom.by, head_box_3d.bottom.b.x, head_box_3d.bottom.b.y + 12, color);
	DrawText("(X) b3 y: ", head_box_3d.bottom.cy, head_box_3d.bottom.c.x, head_box_3d.bottom.c.y + 12, color);
	DrawText("(X) b4 y: ", head_box_3d.bottom.dy, head_box_3d.bottom.d.x, head_box_3d.bottom.d.y + 12, color);
	DrawText("(Y) b1 z: ", head_box_3d.bottom.az, head_box_3d.bottom.a.x, head_box_3d.bottom.a.y + 24, color);
	DrawText("(Y) b2 z: ", head_box_3d.bottom.bz, head_box_3d.bottom.b.x, head_box_3d.bottom.b.y + 24, color);
	DrawText("(Y) b3 z: ", head_box_3d.bottom.cz, head_box_3d.bottom.c.x, head_box_3d.bottom.c.y + 24, color);
	DrawText("(Y) b4 z: ", head_box_3d.bottom.dz, head_box_3d.bottom.d.x, head_box_3d.bottom.d.y + 24, color);
	// Top.
	DrawText("(Z) t1 x: ", head_box_3d.top.ax, head_box_3d.top.a.x, head_box_3d.top.a.y, color);
	DrawText("(Z) t2 x: ", head_box_3d.top.bx, head_box_3d.top.b.x, head_box_3d.top.b.y, color);
	DrawText("(Z) t3 x: ", head_box_3d.top.cx, head_box_3d.top.c.x, head_box_3d.top.c.y, color);
	DrawText("(Z) t4 x: ", head_box_3d.top.dx, head_box_3d.top.d.x, head_box_3d.top.d.y, color);
	DrawText("(X) t1 y: ", head_box_3d.top.ay, head_box_3d.top.a.x, head_box_3d.top.a.y + 12, color);
	DrawText("(X) t2 y: ", head_box_3d.top.by, head_box_3d.top.b.x, head_box_3d.top.b.y + 12, color);
	DrawText("(X) t3 y: ", head_box_3d.top.cy, head_box_3d.top.c.x, head_box_3d.top.c.y + 12, color);
	DrawText("(X) t4 y: ", head_box_3d.top.dy, head_box_3d.top.d.x, head_box_3d.top.d.y + 12, color);
	DrawText("(Y) t1 z: ", head_box_3d.top.az, head_box_3d.top.a.x, head_box_3d.top.a.y + 24, color);
	DrawText("(Y) t2 z: ", head_box_3d.top.bz, head_box_3d.top.b.x, head_box_3d.top.b.y + 24, color);
	DrawText("(Y) t3 z: ", head_box_3d.top.cz, head_box_3d.top.c.x, head_box_3d.top.c.y + 24, color);
	DrawText("(Y) t4 z: ", head_box_3d.top.dz, head_box_3d.top.d.x, head_box_3d.top.d.y + 24, color);
}