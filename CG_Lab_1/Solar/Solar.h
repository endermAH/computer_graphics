#pragma once
#include "Planet.h"
#include "../Framework/GameSystem/Game.h"

class Camera {
public:
	DirectX::XMVECTOR position_ = DirectX::XMVectorSet( 0.0f, 3.0f, -10.0f, 0.0f );

	float at_x_ = 0;
	float at_y_ = 1;
	float at_z_ = 0;

	float yaw_;
	float pitch_;
		
	float r_ = 5;

	void Rotate(float yaw, float pitch) {
		yaw_ = yaw;
		pitch_ = pitch;
	}

	void RotatePitch(float direction) {
		pitch_ += direction;
	}

	void Move(float x, float y, float z) {
		DirectX::XMVECTOR move = DirectX::XMVectorSet(x, y, z, 0.0f);

		float l_pitch = pitch_ * 0.0174532925f;
		float l_yaw = yaw_ * 0.0174532925f;
		
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(l_yaw, l_pitch, 0);
		DirectX::XMVECTOR rotated_move = XMVector3TransformCoord(move, rotationMatrix);

		 position_ = DirectX::XMVectorAdd(position_, rotated_move);
	}
	
	DirectX::XMMATRIX GetView(){
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		
		float l_pitch = pitch_ * 0.0174532925f;
		float l_yaw = yaw_ * 0.0174532925f;
		
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(l_yaw, l_pitch, 0);
		
		lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
		up = XMVector3TransformCoord(up, rotationMatrix);
		
		lookAt = DirectX::XMVectorAdd(position_, lookAt);
		
		return DirectX::XMMatrixLookAtLH(position_, lookAt, up);
	}
};

class Solar : public Game
{
public:
	Camera* camera_;
public:
	Solar(float window_width, float window_height)
		: Game(window_width, window_height, Render::ProjectionType::Perspective) {}
	
	void GameStart() {
		camera_ = new Camera();
		
		PlanetColors yellow;
		yellow.main = DirectX::XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f );
		yellow.border = DirectX::XMFLOAT4( 1.0f, 0.5f, 0.0f, 1.0f );
		yellow.center = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );

		PlanetColors blue_eye;
		blue_eye.border = DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );
		blue_eye.center = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
		blue_eye.main = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

		PlanetColors red_eye;
		red_eye.border = DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
		red_eye.center = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
		red_eye.main = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

		PlanetColors gray;
		gray.border = DirectX::XMFLOAT4( 0.7f, 0.7f, 0.7f, 1.0f );
		gray.center = DirectX::XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
		gray.main = DirectX::XMFLOAT4( 0.7f, 0.7f, 0.7f, 1.0f );
		
		PlanetRotationAround r0;
		r0.rotate_around_radius = 0;
		r0.rotate_around_x = 0;
		r0.rotate_around_y = 0;
		r0.rotate_around_speed = 1;
		
		auto* sun = new Planet(r0, yellow, 2, nullptr);
		sun->is_rotating_around_ = false;
		AddObject(sun);

		PlanetRotationAround r1;
		r1.rotate_around_radius = 4;
		r1.rotate_around_x = &sun->x_;
		r1.rotate_around_y = &sun->y_;
		r1.rotate_around_speed = 2;

		auto* p1 = new Planet(r1, blue_eye, 0.8f, sun);
		AddObject(p1);

		PlanetRotationAround r2;
		r2.rotate_around_radius = 1.5;
		r2.rotate_around_x = &p1->x_;
		r2.rotate_around_y = &p1->y_;
		r2.rotate_around_speed = 2;

		auto* sp1 = new Planet(r2, gray, 0.4f, p1);
		AddObject(sp1);

		PlanetRotationAround r3;
		r3.rotate_around_radius = 7;
		r3.rotate_around_x = &p1->x_;
		r3.rotate_around_y = &p1->y_;
		r3.rotate_around_speed = 3;

		auto* p2 = new Planet(r3, red_eye, 1.2f, sun);
		AddObject(p2);
		
		Run();
		Close();
	}
	
	bool GameFrame() {
		UpdateCamera();
		return true;
	};
	
	void UpdateCamera() {
		RenderSystem* rs = static_cast<RenderSystem*>(render_);

		float factor = GetWindowWidth() / 4 / 360;
		float yaw = (Mouse()->mouse_x_ - GetWindowWidth() / 2) * factor;
		float pitch = (Mouse()->mouse_y_ - GetWindowWidth() / 2) * factor;
		
		camera_->Rotate(pitch, yaw);
		
		float translation_x = 0;
		float translation_y = 0;
		float translation_z = 0;

		float speed_ = 0.3;
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_D)) {
			translation_x = speed_;
		}
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_A)) {
			translation_x = -speed_;
		}
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_W)) {
			translation_z = speed_;
		}
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_S)) {
			translation_z = -speed_;
		}
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_Q)) {
			translation_y = speed_;
		}
		if (Keyboard()->IsKeyDown(eKeyCodes::KEY_E)) {
			translation_y = -speed_;
		}
		camera_->Move(translation_x, translation_y, translation_z);
		
		rs->view_ = camera_->GetView();
		
	}
	
};
