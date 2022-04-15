#pragma once

#include "canvas.h"

extern SDK* sdk;

class Overlay {
public:
	Ent* ent;
	void Display();
	void ShowClientInfo();
	void ShowEntsInfo();
	struct {
		bool show_client_info = false;
		bool show_ents_info = false;
		// Ents config.
		struct {
			bool is_show_bone_ids = false;
			bool is_show_player_model_name = false;
			bool is_show_entity_id_info = false;
			bool is_show_headbox_2d_coords = false;
			bool is_show_headbox_3d_coords = false;
		} ents;
		// Client config.
		struct {
			bool is_show_crosshair_position = false;
		} client;
	} config;

protected:
	void show_bone_ids(Ent* ent);
	void show_crosshair_position();
	void show_player_model_name(Ent* ent, int pos);
	void show_entity_id_info(Ent* ent, int pos);
	void show_headbox_2d(Ent* ent);
	void show_headbox_3d(Ent* ent);
};