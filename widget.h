#pragma once

#include "menu.h"
#include <string>

enum ItemType {
	NAV_CLOSE_MENU,
	NAV_OPEN_CHILD,
	BTN_TOGGLE,
	BTN_PUSH,
	TXT_LABEL,
	SPACER,
	NONE
};

class Item {
public:
	ItemType item_type_ = NONE;
	bool is_accessible_ = true;
	std::string text_ = "";
	std::string extra_text_ = "";
	virtual void RightAction() = 0;
	virtual void LeftAction() = 0;
	virtual int GetStatusCode() = 0;
	virtual bool GetFlag() = 0;
};

class NavCloseMenu : public Item {
public:
	NavCloseMenu(std::string text) {
		item_type_ = NAV_CLOSE_MENU;
		text_ = text;
	};

	virtual void LeftAction() {

	}

	virtual void RightAction() {

	}

	virtual int GetStatusCode() {
		return 30035; // Boobs.
	}

	virtual bool GetFlag() {
		return false;
	}
};

class NavOpenChild : public Item {
public:
	NavOpenChild(std::string text, int code) {
		item_type_ = NAV_OPEN_CHILD;
		text_ = text;
		status_code = code;
	}

	virtual void LeftAction() {

	}

	virtual void RightAction() {

	}

	virtual int GetStatusCode() {
		return status_code;
	}

	virtual bool GetFlag() {
		return false;
	}

private:
	int status_code;
};

class BtnToggle : public Item {
public:
	BtnToggle(std::string text, bool* toggle) {
		item_type_ = BTN_TOGGLE;
		flag = toggle;
		text_ = text;
		if (*toggle == true) extra_text_ = "on";
		else extra_text_ = "off";
	}

	virtual void LeftAction() {
		*flag = false;
		extra_text_ = "off";
	}

	virtual void RightAction() {
		*flag = true;
		extra_text_ = "on";
	}

	virtual int GetStatusCode() {
		return 30035;
	}

	virtual bool GetFlag() {
		return *flag;
	}

private:
	bool* flag = nullptr;
};

class TxtLabel : public Item {
public:
	TxtLabel(std::string text, std::string extra_text) {
		item_type_ = TXT_LABEL;
		text_ = text;
		extra_text_ = extra_text;
		is_accessible_ = false;
	}

	virtual void LeftAction() {

	}

	virtual void RightAction() {

	}

	virtual int GetStatusCode() {
		return 30035;
	}

	virtual bool GetFlag() {
		return false;
	}
};

class Spacer : public Item {
public:
	Spacer() {
		item_type_ = SPACER;
		is_accessible_ = false;
	}

	virtual void LeftAction() {

	}

	virtual void RightAction() {

	}

	virtual int GetStatusCode() {
		return 30035;
	}

	virtual bool GetFlag() {
		return false;
	}
};