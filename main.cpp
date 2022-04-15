#include "includes.h"
#include "hook.h"
#include "canvas.h"
#include "info_overlay.h"
#include "menu.h"
#include <vector>

void* d3d9Device[119];
BYTE EndSceneBytes[7] {0};
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

Overlay* overlay;
SDK* sdk;

Menu* menu;
std::vector<Menu*> menus;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 opDevice) {
	if (!pDevice) pDevice = opDevice;
	DrawText("Ben Dover", window_width / 2, 70, sdk->colors.color_purple);
	// ==================
	//	Entity list loop.
	// ==================
	for (int i = 0; i < 32; i++) {
		Ent* cur_ent = (Ent*) sdk->entity_list->GetClientEntity(i);
		if (!sdk->is_valid_ent(cur_ent)) continue;
		if (sdk->config.show_boneIDs) DrawBoneIDs(cur_ent);
		if (sdk->config.bone_esp) DrawBoneEsp(cur_ent);
		Vec3 ent_head_3d = sdk->get_bone_pos(cur_ent, 8); ent_head_3d.z += 8;
		Vec2 ent_pos_2d, ent_head_2d;
		if (sdk->world_to_screen(cur_ent->m_vecOrigin, ent_pos_2d)) {
			if (sdk->world_to_screen(ent_head_3d, ent_head_2d)) {
				// 2D head box esp.
				if (sdk->config.head_box_esp_2d) {
					Vec2 ent_bone_7_pos;
					if (W2S(sdk->get_bone_pos(cur_ent, 7), ent_bone_7_pos)) {
						DrawHeadBox2D(ent_head_2d, ent_bone_7_pos, ent_head_2d, 2, sdk->colors.color_green);
					}
				}
				// 3D head box esp.
				if (sdk->config.head_box_esp_3d) {
					DrawHeadBox3D(ent_head_3d, sdk->get_bone_pos(cur_ent, 7), cur_ent->m_angEyeAnglesY, 25, 2, sdk->colors.color_green);
				}
			}
		}
	}
	overlay->Display();
	for (Menu* m : menus) m->Show();
	oEndScene(pDevice);
}

// =============
//	Trigger Bot.
// =============
void TriggerBot() {
	uintptr_t local_player = sdk->get_local_player();
	int crosshair = *(int*) (local_player + offsets::m_iCrosshairId);
	if (crosshair != 0 && crosshair < 32) {
		Ent* entity = (Ent*) sdk->entity_list->GetClientEntity(crosshair);
		int entity_team = entity->m_iTeamNum;
		int entity_health = entity->m_iHealth;
		int local_player_team = *(int*) (local_player + offsets::m_iTeamNum);
		if (entity_health > 0) {
			// Body shots.
			if (!sdk->config.trigger_hs_only) {
				*(int*) (sdk->client + offsets::dwForceAttack) = 5;
				Sleep(1);
				*(int*) (sdk->client + offsets::dwForceAttack) = 4;
				return;
			}
			Vec3 punch_angle = *(Vec3*)(uintptr_t*)(local_player + offsets::m_aimPunchAngle);
			Vec2 crosshair_position;
			crosshair_position.x = window_width / 2 - (window_width / 90 * punch_angle.y);
			crosshair_position.y = window_height / 2 + (window_height / 90 * punch_angle.x);
			Vec3 ent_head_3d = sdk->get_bone_pos(entity, 8);
			Vec2 ent_head_2d;
			if (sdk->world_to_screen(ent_head_3d, ent_head_2d)) {
				Vec2 ent_bone_7_pos;
				Rect head_box;
				if (W2S(sdk->get_bone_pos(entity, 7), ent_bone_7_pos)) {
					head_box = sdk->headbox_2D(ent_bone_7_pos, ent_head_2d);
					// Head shots.
					if (crosshair_position.y < head_box.top_left.y) {
						*(int*) (sdk->client + offsets::dwForceAttack) = 5;
						Sleep(1);
						*(int*) (sdk->client + offsets::dwForceAttack) = 4;
					}
				}
			}
		}
	}
}

// ===============
//	Player Moving.
// ===============
bool IsPlayerMoving(uintptr_t local_player) {
	Vec3 player_velocity = *(Vec3*) (local_player + offsets::m_vecVelocity);
	return (player_velocity.Sum() != 0);
}

// ===========
//	Bunny Hop.
// ===========
void BunnyHop() {
	uintptr_t local_player = sdk->get_local_player();
	if (local_player != NULL) {
		if (IsPlayerMoving(local_player)) {
			BYTE flag = *(BYTE*) (local_player + offsets::m_fFlags);
			uintptr_t current_weapon = *(uintptr_t*) (local_player + offsets::m_hActiveWeapon) & 0xfff;
			current_weapon = *(uintptr_t*) (sdk->client + offsets::dwEntityList + (current_weapon - 1) * 0x10);
			int weapon_id = *(int*) (current_weapon + offsets::m_iItemDefinitionIndex);
			if (flag != NULL && weapon_id != NULL)
				//if ((weapon_id == 41 || weapon_id == 42 || weapon_id == 59) && flag & (1 << 0))
				if (flag & (1 << 0))
					*(uintptr_t*) (sdk->client + offsets::dwForceJump) = 6;
		}
	}
}

// ===============
//	Setup For sdk.
// ===============
DWORD WINAPI SDKThread(HMODULE hModule) {
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*) d3d9Device[42], 7);
		oEndScene = (tEndScene) TrampolineHook((char*) d3d9Device[42], (char*) hkEndScene, 7);
	}
	overlay = new Overlay();
	sdk = new SDK();
	sdk->init();
	struct {
		int key_up = 0;
		int key_down = -32768;
		bool flag_nav_up = false;
		bool flag_nav_down = false;
		bool flag_nav_left = false;
		bool flag_nav_right = false;
	} menu_nav;
	// Hack Menu => Root Menu.
	std::vector<Item*> items_hack;
	items_hack.push_back(new NavCloseMenu("Close"));
	items_hack.push_back(new NavOpenChild("Debug Overlay", 0));
	items_hack.push_back(new Spacer);
	items_hack.push_back(new TxtLabel("Quick Config", ""));
	items_hack.push_back(new BtnToggle("Trigger HS only:", &sdk->config.trigger_hs_only));
	items_hack.push_back(new Spacer);
	items_hack.push_back(new TxtLabel("Hacks", ""));
	items_hack.push_back(new BtnToggle("Bone ESP:", &sdk->config.bone_esp));
	items_hack.push_back(new BtnToggle("Trigger Bot:", &sdk->config.trigger_bot));
	items_hack.push_back(new BtnToggle("Bunny Hop:", &sdk->config.bunny_hop));
	items_hack.push_back(new Spacer);
	items_hack.push_back(new TxtLabel("Others", ""));
	items_hack.push_back(new NavOpenChild("Other ESP", 1));
	items_hack.push_back(new Spacer);
	items_hack.push_back(new TxtLabel("Hack Settings", ""));
	items_hack.push_back(new BtnToggle("Bunny Hop toggle key:", &sdk->config.bunny_hop_n6));
	Menu* menu_hack = new Menu("Dank Hacks", sdk->colors.menu_yellow, items_hack);
	// Debug Menu => (0) Hack Menu.
	std::vector<Item*> items_debug;
	items_debug.push_back(new NavCloseMenu("Back"));
	items_debug.push_back(new Spacer);
	items_debug.push_back(new TxtLabel("Overlays", ""));
	items_debug.push_back(new BtnToggle("Show client info:", &overlay->config.show_client_info));
	items_debug.push_back(new BtnToggle("Show entity info:", &overlay->config.show_ents_info));
	items_debug.push_back(new Spacer);
	items_debug.push_back(new TxtLabel("Config", ""));
	items_debug.push_back(new BtnToggle("Crosshair pos:", &overlay->config.client.is_show_crosshair_position));
	items_debug.push_back(new BtnToggle("Entity ID info:", &overlay->config.ents.is_show_entity_id_info));
	items_debug.push_back(new BtnToggle("Player model names:", &overlay->config.ents.is_show_player_model_name));
	items_debug.push_back(new BtnToggle("Entity bone IDs:", &overlay->config.ents.is_show_bone_ids));
	items_debug.push_back(new BtnToggle("Head box 2D coords:", &overlay->config.ents.is_show_headbox_2d_coords));
	items_debug.push_back(new BtnToggle("Head box 3D coords:", &overlay->config.ents.is_show_headbox_3d_coords));
	items_debug.push_back(new Spacer);
	items_debug.push_back(new TxtLabel("Caution for show client info", ""));
	Menu* menu_debug = new Menu("Debug Overlay", sdk->colors.color_purple, items_debug);
	menu_debug->SetParent(menu_hack);
	// Other ESP Menu => (1) Hack Menu.
	std::vector<Item*> items_other_esp;
	items_other_esp.push_back(new NavCloseMenu("Back"));
	items_other_esp.push_back(new Spacer);
	items_other_esp.push_back(new TxtLabel("Other ESP", ""));
	items_other_esp.push_back(new BtnToggle("Head box esp 2D:", &sdk->config.head_box_esp_2d));
	items_other_esp.push_back(new BtnToggle("Head box esp 3D:", &sdk->config.head_box_esp_3d));
	items_other_esp.push_back(new Spacer);
	items_other_esp.push_back(new TxtLabel("Dank", ""));
	Menu* menu_other_esp = new Menu("Other ESP", sdk->colors.color_blue, items_other_esp);
	menu_other_esp->SetParent(menu_hack);
	// Menu list.
	menus.push_back(menu_hack);
	menus.push_back(menu_debug);
	menus.push_back(menu_other_esp);
	// Root menu.
	menu = menu_hack;
	menu->SetAsRoot();
	// ===========
	//	sdk Loop.
	// ===========
	while (!GetAsyncKeyState(VK_END)) {
		sdk->update();
		if (sdk->config.trigger_bot && (GetAsyncKeyState(0x34) & 0x8000)) TriggerBot();
		if (sdk->config.bunny_hop && GetAsyncKeyState(VK_SPACE) && (sdk->config.bunny_hop_n6 ? (GetAsyncKeyState(0x36) & 0x8000) : true)) BunnyHop();
		// ==== Menu Nav Top =========
		if (GetAsyncKeyState(VK_DELETE)) {
			menu->selected_item = 0;
		}
		// ===========================
		// ==== Menu Nav Up ==========
		if (GetAsyncKeyState(VK_UP) == menu_nav.key_down) {
			if (menu_nav.flag_nav_up == false) menu->ItemUp();
			menu_nav.flag_nav_up = true;
		}
		if (GetAsyncKeyState(VK_UP) == menu_nav.key_up) {
			menu_nav.flag_nav_up = false;
		}
		// ===========================
		// ==== Menu Nav Down ========
		if (GetAsyncKeyState(VK_DOWN) == menu_nav.key_down) {
			if (menu_nav.flag_nav_down == false) menu->ItemDown();
			menu_nav.flag_nav_down = true;
		}
		if (GetAsyncKeyState(VK_DOWN) == menu_nav.key_up) {
			menu_nav.flag_nav_down = false;
		}
		// ===========================
		// ==== Menu Nav Left ========
		if (GetAsyncKeyState(VK_LEFT) == menu_nav.key_down) {
			if (menu_nav.flag_nav_left == false) {
				// Functionality.
				if (menu->items_[menu->selected_item]->item_type_ == NAV_CLOSE_MENU) {
					if (!menu->IsRoot()) menu->is_focused = false;
					menu->is_visible = false;
					if (menu->is_child) {
						menu = menu->parent;
						menu->is_focused = true;
					}
				}
				menu->ItemLeftAction();
			}
			menu_nav.flag_nav_left = true;
		}
		if (GetAsyncKeyState(VK_LEFT) == menu_nav.key_up) {
			menu_nav.flag_nav_left = false;
		}
		// ===========================
		// ==== Menu Nav Right =======
		if (GetAsyncKeyState(VK_RIGHT) == menu_nav.key_down) {
			if (menu_nav.flag_nav_right == false) {
				// Functionality
				if (menu->items_[menu->selected_item]->item_type_ == NAV_CLOSE_MENU) {
					menu->is_visible = true;
				}
				if (menu->items_[menu->selected_item]->item_type_ == NAV_OPEN_CHILD) {
					int menu_to_open = menu->items_[menu->selected_item]->GetStatusCode();
					menu->is_focused = false;
					menu = menu->children[menu_to_open];
					menu->is_visible = true;
					menu->is_focused = true;
				}
				menu->ItemRightAction();
			}
			menu_nav.flag_nav_right = true;
		}
		if (GetAsyncKeyState(VK_RIGHT) == menu_nav.key_up) {
			menu_nav.flag_nav_right = false;
		}
		// ===========================
	}
	// Unhook, uninject.
	Patch((BYTE*) d3d9Device[42], EndSceneBytes, 7);
	Sleep(1000);
	FreeLibraryAndExitThread(hModule, 0);
}

// =============
//	Entry Point.
// =============
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH) CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE) SDKThread, hModule, 0, 0));
	return TRUE;
}