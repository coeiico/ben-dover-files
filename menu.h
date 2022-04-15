#pragma once

#include "widget.h"
#include "canvas.h"
#include "sdk.h"
#include <vector>

extern SDK* sdk;

class Menu {
public:
	bool is_visible = false;
	bool is_focused = false;
	bool is_parent = false;
	bool is_child = false;
	std::vector<Item*> items_;
	int selected_item;
	Menu* parent = nullptr;
	std::vector<Menu*> children;
	Menu(std::string menu_title, D3DCOLOR color_menu_border, std::vector<Item*> items);
	void Show();
	void SetAsRoot();
	bool IsRoot();
	void SetParent(Menu* parent);
	void ItemUp();
	void ItemDown();
	void ItemLeftAction();
	void ItemRightAction();

protected:
	bool increase_selected_item();
	bool decrease_selected_item(int temp_selected_item);

private:
	std::string menu_title_;
	D3DCOLOR color_menu_border_;
	Vec2 start_pos = {20, (window_height / 2.f) - 80.f};
	int width = 260;
	int height;
	int item_count;
	bool is_root_menu = false;
};