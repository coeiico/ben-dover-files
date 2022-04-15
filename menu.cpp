#include "menu.h"

Menu::Menu(std::string menu_title, D3DCOLOR color_menu_border, std::vector<Item*> items) {
	menu_title_ = menu_title;
	items_ = items;
	color_menu_border_ = color_menu_border;
	item_count = items.size();
	height = 20 + 50 + (item_count * 20);
	selected_item = 0;
}

void Menu::SetParent(Menu* parent) {
	parent->children.push_back(this);
	parent->is_parent = true;
	start_pos.x = parent->start_pos.x + width + 20;
	this->parent = parent;
	is_child = true;
}

void Menu::Show() {
	if (!is_visible) return;
	DrawFilledRect(start_pos.x, start_pos.y, width, height, color_menu_border_);
	DrawFilledRect(start_pos.x + 4, start_pos.y + 4, width - 8, height - 8, sdk->colors.menu_background);
	TextAlignFormat text_align_formats;
	DrawMenuText((char*) menu_title_.c_str(), start_pos.x + 20, start_pos.y + 20, 25, sdk->colors.menu_orange, text_align_formats.left);
	int item_pos = 0;
	for (int i = 0; i < item_count; i++) {
		D3DCOLOR color_extra_text = sdk->colors.menu_skin;
		D3DCOLOR color_text = sdk->colors.menu_skin;
		if (items_[i]->item_type_ == BTN_TOGGLE)
			if (items_[i]->GetFlag() == true) color_extra_text = sdk->colors.menu_green;
			else color_extra_text = sdk->colors.menu_red;
		if (items_[i]->item_type_ == TXT_LABEL)
			color_text = sdk->colors.menu_gray_orange;
		if (i == selected_item) {
			std::string text = "> " + items_[i]->text_;
			DrawMenuText((char*) items_[i]->extra_text_.c_str(),
				start_pos.x + width - 28, start_pos.y + 50 + (item_pos * 20),
				20, color_extra_text, text_align_formats.right);
			DrawMenuText((char*) text.c_str(),
				start_pos.x + 20, start_pos.y + 50 + (item_pos++ * 20),
				20, sdk->colors.menu_white, text_align_formats.left);
		} else {
			DrawMenuText((char*) items_[i]->extra_text_.c_str(),
				start_pos.x + width - 20, start_pos.y + 50 + (item_pos * 20),
				20, color_extra_text, text_align_formats.right);
			DrawMenuText((char*) items_[i]->text_.c_str(),
				start_pos.x + 20, start_pos.y + 50 + (item_pos++ * 20),
				20, color_text, text_align_formats.left);
		}
		// Dank menu deugging => Show position of selected item.
		//std::string item_pos = "[ " + std::to_string(selected_item) + " ]";
		//DrawMenuText((char*) item_pos.c_str(), start_pos.x + width - 20, start_pos.y + 20, 20, sdk->colors.menu_white, text_align_formats.right);
	}
}

bool Menu::increase_selected_item() {
	if (selected_item <= item_count - 1 && selected_item > 0) {
		if (items_[selected_item - 1]->is_accessible_) {
			selected_item--;
			return true;
		} else {
			selected_item--;
			return increase_selected_item();
		}
	} else return false;
}

bool Menu::decrease_selected_item(int temp_selected_item) {
	if (selected_item < item_count - 1 && selected_item >= 0) {
		if (items_[selected_item + 1]->is_accessible_) {
			selected_item++;
			return true;
		} else {
			selected_item++;
			return decrease_selected_item(temp_selected_item);
		}
	} else {
		selected_item = temp_selected_item;
		return false;
	}
}

void Menu::ItemUp() {
	if (is_visible)
		increase_selected_item();
}

void Menu::ItemDown() {
	if (is_visible)
		decrease_selected_item(selected_item);
}

void Menu::ItemLeftAction() {
	if (is_focused)
		items_[selected_item]->LeftAction();
}

void Menu::ItemRightAction() {
	if (is_focused)
		items_[selected_item]->RightAction();
}

void Menu::SetAsRoot() {
	is_visible = true;
	is_focused = true;
	is_root_menu = true;
}

bool Menu::IsRoot() {
	return is_root_menu;
}