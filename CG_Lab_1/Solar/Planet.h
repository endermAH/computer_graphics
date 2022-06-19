#pragma once

#include "RenderPlanet.h"
#include "../Framework/stdafx.h"
#include "../Framework/GameSystem/GameObject.h"

struct PlanetRotationAround
{
	float rotate_around_radius;
	float* rotate_around_x;
	float* rotate_around_y;
	float rotate_around_speed;
};

class Planet : public GameObject
{
public:
	const float radius_ = 1;

	Planet* parent_planet_;
	
	float speed_ = 0.2f;
	float x_ = 0.0f;
	float y_ = 0.0f;

	PlanetRotationAround rotation_around;
	
	bool is_rotating_around_ = true;
	float* rotate_around_x_;
	float* rotate_around_y_;
	float rotate_around_radius_ = 1;
	float rotate_around_speed_ = 1;

	float scale_ = 1;
	
	Planet(PlanetRotationAround rotation_around, PlanetColors colors, float scale, Planet* parent_planet) {
		scale_ = scale;
		
		render_object_ = new RenderPlanet(radius_ * scale_, colors);

		rotate_around_x_ = rotation_around.rotate_around_x;
		rotate_around_y_ = rotation_around.rotate_around_y;
		rotate_around_radius_ = rotation_around.rotate_around_radius;
		rotate_around_speed_ = rotation_around.rotate_around_speed;
		parent_planet_ = parent_planet;
	}
	
	void Update() {
		
		static float t = 0.0f;
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / 1000.0f;

		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationY( -t );

		render_object_->world_ = rotation_matrix;

		if (is_rotating_around_) {
			DirectX::XMMATRIX translation_matrix = DirectX::XMMatrixTranslation( rotate_around_radius_ , 0, 0.0f );
			DirectX::XMMATRIX rotation_around_matrix = DirectX::XMMatrixRotationY( -t * rotate_around_speed_ );
			render_object_->world_ *= translation_matrix * rotation_around_matrix;
		}
		
		if (parent_planet_ != nullptr) {
			render_object_->world_ *= parent_planet_->render_object_->world_;
		} /*else {
			render_object_->world_ = DirectX::XMMatrixIdentity();//rotation_matrix;
		}*/
		

		
	}
	
};
