#include "sdk.h"

void* GetInterface(const char* dllname, const char* interfacename);

void SDK::init() {
	//memcpy(&view_matrix, (PBYTE*) (client + offsets::dwViewMatrix), sizeof(view_matrix));
	client = (uintptr_t)GetModuleHandleA("client.dll");
	engine = (uintptr_t)GetModuleHandleA("engine.dll");
}

void SDK::update() {
	memcpy(&view_matrix, (PBYTE*)(client + offsets::dwViewMatrix), sizeof(view_matrix));
	entity_list = (ent_list::IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
}

uintptr_t SDK::get_local_player() {
	// uintptr_t client = (uintptr_t) GetModuleHandleA("client_panorama.dll");
	// uintptr_t localPlayer = *(uintptr_t*) (client + offsets::dwLocalPlayer);
	return *(uintptr_t*)(client + offsets::dwLocalPlayer);
}

SDK::~SDK() {
	this->FontF->Release();
	this->LineL->Release();
}

bool SDK::is_valid_ent(Ent* ent) {
	if (ent == nullptr) return false;
	if ((uintptr_t) ent == get_local_player()) return false;
	if (ent->m_iHealth <= 0) return false;
	if (ent->m_bDormant) return false;
	return true;
}

bool SDK::world_to_screen(Vec3 coords_3d, Vec2& coords_2d) {
	Vec4 clip_coords;
	clip_coords.x = coords_3d.x * view_matrix[0] + coords_3d.y * view_matrix[1] + coords_3d.z * view_matrix[2] + view_matrix[3];
	clip_coords.y = coords_3d.x * view_matrix[4] + coords_3d.y * view_matrix[5] + coords_3d.z * view_matrix[6] + view_matrix[7];
	clip_coords.z = coords_3d.x * view_matrix[8] + coords_3d.y * view_matrix[9] + coords_3d.z * view_matrix[10] + view_matrix[11];
	clip_coords.w = coords_3d.x * view_matrix[12] + coords_3d.y * view_matrix[13] + coords_3d.z * view_matrix[14] + view_matrix[15];
	if (clip_coords.w < 0.1f) return false;
	Vec3 NDC;
	NDC.x = clip_coords.x / clip_coords.w;
	NDC.y = clip_coords.y / clip_coords.w;
	NDC.z = clip_coords.z / clip_coords.w;
	coords_2d.x = (window_width / 2 * NDC.x) + (NDC.x + window_width / 2);
	coords_2d.y = -(window_height / 2 * NDC.y) + (NDC.y + window_height / 2);
	return true;
}

Vec2 SDK::world_to_screen(Vec3 coords_3d) {
	Vec4 clip_coords;
	clip_coords.x = coords_3d.x * view_matrix[0] + coords_3d.y * view_matrix[1] + coords_3d.z * view_matrix[2] + view_matrix[3];
	clip_coords.y = coords_3d.x * view_matrix[4] + coords_3d.y * view_matrix[5] + coords_3d.z * view_matrix[6] + view_matrix[7];
	clip_coords.z = coords_3d.x * view_matrix[8] + coords_3d.y * view_matrix[9] + coords_3d.z * view_matrix[10] + view_matrix[11];
	clip_coords.w = coords_3d.x * view_matrix[12] + coords_3d.y * view_matrix[13] + coords_3d.z * view_matrix[14] + view_matrix[15];
	Vec3 NDC;
	NDC.x = clip_coords.x / clip_coords.w;
	NDC.y = clip_coords.y / clip_coords.w;
	NDC.z = clip_coords.z / clip_coords.w;
	Vec2 coords_2d;
	coords_2d.x = (window_width / 2 * NDC.x) + (NDC.x + window_width / 2);
	coords_2d.y = -(window_height / 2 * NDC.y) + (NDC.y + window_height / 2);
	return coords_2d;
}

Vec3 SDK::get_bone_pos(Ent* ent, int bone) {
	uintptr_t bone_ptr = ent->m_dwBoneMatrix;
	Vec3 bone_pos;
	bone_pos.x = *(float*)(bone_ptr + 0x30 * bone + 0x0C);
	bone_pos.y = *(float*)(bone_ptr + 0x30 * bone + 0x1C);
	bone_pos.z = *(float*)(bone_ptr + 0x30 * bone + 0x2C);
	return bone_pos;
}

Vec3 SDK::transform_vec(Vec3 src, Vec3 ang, float d) {
	Vec3 new_pos;
	new_pos.x = src.x + (cosf(TORAD(ang.y)) * d);
	new_pos.y = src.y + (sinf(TORAD(ang.y)) * d);
	new_pos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return new_pos;
}

Rect SDK::headbox_2D(Vec2 top, Vec2 bottom) {
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
	return {
		{ top_left.x, top_left.y },
		{ top_right.x, top_right.y },
		{ bottom_left.x, bottom_left.y },
		{ bottom_right.x, bottom_right.y }
	};
}

HyperRect SDK::headbox_3D(Ent* ent) {
	Vec3 top = get_bone_pos(ent, 8); top.z += 8;
	Vec3 bottom = get_bone_pos(ent, 7);
	float angle = ent->m_angEyeAnglesY;
	int width = 25;
	int height3D = std::abs(top.z - bottom.z);
	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.x = bottom.x + (cos(TORAD(angle + 45)) * width / 2);		b2.x = bottom.x + (cos(TORAD(angle + 135)) * width / 2);
	b1.y = bottom.y + (sin(TORAD(angle + 45)) * width / 2);		b2.y = bottom.y + (sin(TORAD(angle + 135)) * width / 2);
	b1.z = bottom.z;											b2.z = bottom.z;
	b3.x = bottom.x + (cos(TORAD(angle + 225)) * width / 2);	b4.x = bottom.x + (cos(TORAD(angle + 315)) * width / 2);
	b3.y = bottom.y + (sin(TORAD(angle + 225)) * width / 2);	b4.y = bottom.y + (sin(TORAD(angle + 315)) * width / 2);
	b3.z = bottom.z;											b4.z = bottom.z;
	t1.x = b1.x;						t2.x = b2.x;
	t1.y = b1.y;						t2.y = b2.y;
	t1.z = b1.z + height3D;				t2.z = b2.z + height3D;
	t3.x = b3.x;						t4.x = b4.x;
	t3.y = b3.y;						t4.y = b4.y;
	t3.z = b3.z + height3D;				t4.z = b4.z + height3D;
	Vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	HyperRect head_box_3d = {
		{
			{ 0, 0 },
			0, 0, 0,
			{ 0, 0 },
			0, 0, 0,
			{ 0, 0 },
			0, 0, 0
		},
		{
			{ 0, 0 },
			0, 0, 0,
			{ 0, 0 },
			0, 0, 0,
			{ 0, 0 },
			0, 0, 0
		}
	};
	if (world_to_screen(b1, b1_2) && world_to_screen(b2, b2_2) && world_to_screen(b3, b3_2)
		&& world_to_screen(b4, b4_2) && world_to_screen(t1, t1_2) && world_to_screen(t2, t2_2)
		&& world_to_screen(t3, t3_2) && world_to_screen(t4, t4_2)) {
		// Bottom.
		head_box_3d.bottom.a.x = b1_2.x;
		head_box_3d.bottom.a.y = b1_2.y;
		head_box_3d.bottom.ax = b1.x;	// Z
		head_box_3d.bottom.ay = b1.y;	// X
		head_box_3d.bottom.az = b1.z;	// Y
		head_box_3d.bottom.b.x = b2_2.x;
		head_box_3d.bottom.b.y = b2_2.y;
		head_box_3d.bottom.bx = b2.x;	// Z
		head_box_3d.bottom.by = b2.y;	// X
		head_box_3d.bottom.bz = b2.z;	// Y
		head_box_3d.bottom.c.x = b3_2.x;
		head_box_3d.bottom.c.y = b3_2.y;
		head_box_3d.bottom.cx = b3.x;	// Z
		head_box_3d.bottom.cy = b3.y;	// X
		head_box_3d.bottom.cz = b3.z;	// Y
		head_box_3d.bottom.d.x = b4_2.x;
		head_box_3d.bottom.d.y = b4_2.y;
		head_box_3d.bottom.dx = b4.x;	// Z
		head_box_3d.bottom.dy = b4.y;	// X
		head_box_3d.bottom.dz = b4.z;	// Y
		// Top.
		head_box_3d.top.a.x = t1_2.x;
		head_box_3d.top.a.y = t1_2.y;
		head_box_3d.top.ax = t1.x;	// Z
		head_box_3d.top.ay = t1.y;	// X
		head_box_3d.top.az = t1.z;	// Y
		head_box_3d.top.b.x = t2_2.x;
		head_box_3d.top.b.y = t2_2.y;
		head_box_3d.top.bx = t2.x;	// Z
		head_box_3d.top.by = t2.y;	// X
		head_box_3d.top.bz = t2.z;	// Y
		head_box_3d.top.c.x = t3_2.x;
		head_box_3d.top.c.y = t3_2.y;
		head_box_3d.top.cx = t3.x;	// Z
		head_box_3d.top.cy = t3.y;	// X
		head_box_3d.top.cz = t3.z;	// Y
		head_box_3d.top.d.x = t4_2.x;
		head_box_3d.top.d.y = t4_2.y;
		head_box_3d.top.dx = t4.x;	// Z
		head_box_3d.top.dy = t4.y;	// X
		head_box_3d.top.dz = t4.z;	// Y
	}
	return head_box_3d;
}

void* GetInterface(const char* dllname, const char* interfacename) {
	ent_list::tCreateInterface CreateInterface = (ent_list::tCreateInterface) GetProcAddress(GetModuleHandleA(dllname), "CreateInterface");
	int returnCode = 0;
	void* interface_ = CreateInterface(interfacename, &returnCode);
	return interface_;
}