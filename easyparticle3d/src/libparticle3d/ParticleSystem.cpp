#include "ParticleSystem.h"
#include "Symbol.h"
#include "config.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"
#include "utility.h"

namespace eparticle3d
{

// ParticleSystem::ParticleSystem(/*const ParticleSystem& ps*/)
// 	: m_recorder(PARTICLE_CAP)
// {
// 	// todo
// }

ParticleSystem::ParticleSystem(unsigned int buffer)
	: m_anim_recorder(new AnimRecorder(buffer))
	, m_inv_record(new InvertRecord)
{
//	delete[] pStart;
	pLast = pStart = new Particle[buffer];
	pEnd = pStart + buffer;

	active = false;
	m_loop = false;

	emitCounter = 0;

	emission_time = 0;
	count = 0;

	min_life = max_life = 0;

	min_hori = max_hori = 0;
	min_vert = min_vert = 0;

	min_spd = max_spd = 0;
	min_angular_spd = max_angular_spd = 0;

	min_dis_region = max_dis_region = 0;
	min_dis_spd = max_dis_spd = 0;

	gravity = 0;

	min_linear_acc = max_linear_acc = 0;

	inertia = 0;

	fadeout_time = 0;

	bounce = true;

	additive_blend = false;

	start_radius = 0;
	is_start_radius_3d = false;

	orient_to_movement = false;
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	//	delete[] pStart;
	pLast = pStart = new Particle[PARTICLE_CAP];
	pEnd = pStart + PARTICLE_CAP;

	origin = ps.origin;

	filepath = ps.filepath;

	children = ps.children;

	active = ps.active;
	m_loop = ps.m_loop;
	lifetime = ps.lifetime;
	life = ps.life;

	emitCounter = ps.emitCounter;

	emission_time = ps.emission_time;
	count = ps.count;

	direction = ps.direction;

	min_life = ps.min_life;
	max_life = ps.max_life;

	min_hori = ps.min_hori;
	max_hori = ps.max_hori;
	min_vert = ps.min_vert;
	max_vert = ps.max_vert;

	min_spd = ps.min_spd;
	max_spd = ps.max_spd;

	min_angular_spd = ps.min_angular_spd;
	max_angular_spd = ps.max_angular_spd;

	min_dis_region = ps.min_dis_region;
	max_dis_region = ps.max_dis_region;
	min_dis_spd = ps.min_dis_spd;
	max_dis_spd = ps.max_dis_spd;

	gravity = ps.gravity;

	min_linear_acc = ps.min_linear_acc;
	max_linear_acc = ps.max_linear_acc;

	inertia = ps.inertia;

	fadeout_time = ps.fadeout_time;

	bounce = ps.bounce;

	additive_blend = ps.additive_blend;

	start_radius = ps.start_radius;
	is_start_radius_3d = ps.is_start_radius_3d;

	orient_to_movement = ps.orient_to_movement;
}

ParticleSystem::~ParticleSystem()
{
	delete m_anim_recorder;
	delete m_inv_record;

	if (pStart != NULL)
		delete[] pStart;
}

void ParticleSystem::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		count = data.val0;
		break;
	case PS_EMISSION_TIME:
		emission_time = data.val0 * 0.001f; 
		lifetime = emission_time;
		break;
	case PS_LIFE_TIME:
		min_life = (data.val0 - data.val1) * 0.001f;
		max_life = (data.val0 + data.val1) * 0.001f;
		break;
	case PS_SPEED:
		min_spd = (data.val0 - data.val1) * 0.25f;
		max_spd = (data.val0 + data.val1) * 0.25f;
		break;
	case PS_ANGULAR_SPEED:
		min_angular_spd = (data.val0 - data.val1);
		max_angular_spd = (data.val0 + data.val1);
		break;
	case PS_DISTURBANCE_RADIUS:
		min_dis_region = (data.val0 - data.val1);
		max_dis_region = (data.val0 + data.val1);
		break;
	case PS_DISTURBANCE_SPD:
		min_dis_spd = (data.val0 - data.val1);
		max_dis_spd = (data.val0 + data.val1);
		break;
	case PS_GRAVITY:
		gravity = data.val0 * 0.3f;
		m_inv_record->SetGravity(gravity);
		break;
	case PS_LINEAR_ACC:
		min_linear_acc = (data.val0 - data.val1);
		max_linear_acc = (data.val0 + data.val1);
		break;
	case PS_FADEOUT_TIME:
		fadeout_time = data.val0 * 0.001f;
		break;
	case PS_START_RADIUS:
		start_radius = data.val0;
		break;
	}
}

void ParticleSystem::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		data.val0 = count;
		break;
	case PS_EMISSION_TIME:
		data.val0 = emission_time * 1000;
		break;
	case PS_LIFE_TIME:
		data.val0 = (min_life + max_life) * 0.5f * 1000;
		data.val1 = (max_life - min_life) * 0.5f * 1000;
		break;
	case PS_SPEED:
		data.val0 = (min_spd + max_spd) * 0.5f * 4;
		data.val1 = (max_spd - min_spd) * 0.5f * 4;
		break;
	case PS_ANGULAR_SPEED:
		data.val0 = (min_angular_spd + max_angular_spd) * 0.5f;
		data.val1 = (max_angular_spd - min_angular_spd) * 0.5f;
		break;
	case PS_DISTURBANCE_RADIUS:
		data.val0 = (min_dis_region + max_dis_region) * 0.5f;
		data.val1 = (max_dis_region - min_dis_region) * 0.5f;
		break;
	case PS_DISTURBANCE_SPD:
		data.val0 = (min_dis_spd + max_dis_spd) * 0.5f;
		data.val1 = (max_dis_spd - min_dis_spd) * 0.5f;
		break;
	case PS_GRAVITY:
		data.val0 = gravity / 0.3f;
		break;
	case PS_LINEAR_ACC:
		data.val0 = (min_linear_acc + max_linear_acc) * 0.5f;
		data.val1 = (max_linear_acc - min_linear_acc) * 0.5f;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = fadeout_time * 1000;
		break;
	case PS_START_RADIUS:
		data.val0 = start_radius;
		break;
	}
}

void ParticleSystem::draw(const d2d::Matrix& mt, AnimRecorder* recorder)
{
	if (m_anim_recorder) {
		m_anim_recorder->FinishFrame();
	}

	vertex vertices[4];

	Particle* p = pStart;
	while (p != pLast)
	{
		//glPushAttrib(GL_CURRENT_BIT);

		d2d::Colorf mul_col = p->pc->mul_col,
			add_col = p->pc->add_col;
		if (p->life < fadeout_time)
		{
			unsigned char a = 255 * p->life / fadeout_time;
			vertices[0].a = vertices[1].a = vertices[2].a = vertices[3].a = a;
			mul_col.a = a / 255.0f;
		}

		d2d::Vector pos = TransCoords3To2(p->position, direction);
		
		float proc = (p->lifetime - p->life) / p->lifetime;
		float s = proc * (p->pc->end_scale - p->pc->start_scale) + p->pc->start_scale;
		float alpha = proc * (p->pc->end_alpha - p->pc->start_alpha) + p->pc->start_alpha;
		mul_col.a *= alpha;

		d2d::Matrix _mt(mt);
		_mt.translate(p->pos.x, p->pos.y);
		d2d::SpriteRenderer::Instance()->Draw(p->pc->symbol, _mt, pos, p->angle, s, s, 0, 0, mul_col, add_col);

		if (p->m_bind_ps) {
			d2d::Matrix _mt;
			_mt.translate(p->pos.x, p->pos.y);
			p->m_bind_ps->draw(_mt, m_anim_recorder);
		}

		AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
		if (curr_record) {
			d2d::Vector fixed = d2d::Math::transVector(pos, _mt);
			curr_record->AddItem(p->pc->symbol->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
		}

		//glPopAttrib();

		++p;
	}
}

void ParticleSystem::update(float dt)
{
	if (active)
	{
		float rate = emission_time / count;
		emitCounter += dt;
		while (emitCounter > rate)
		{
			add();
			emitCounter -= rate;
		}

		if (!m_loop) {
 			life -= dt;
 			if (lifetime != -1 && life < 0) {
 				stop();
 			}
		}
	}

	Particle* p = pStart;
	while (p != pLast)
	{
// 		if (p->pc->m_bind_ps) {
// 			p->pc->m_bind_time += dt;
// 			ParticleSystem* cps = p->pc->m_bind_ps;
// 			if (p->pc->m_bind_time > 0.05f) {
// 				p->pc->m_bind_time = 0;
// 				cps->origin = TransCoords3To2(p->position);
// 				cps->start();
// 			}
// 			cps->update(dt);
// 		}

		if (p->m_bind_ps) {
			ParticleSystem* cps = p->m_bind_ps;
			cps->update(dt);

			p->m_bind_time += dt;
			if (p->m_bind_time > 0.05f) {
				p->m_bind_time = 0;
				cps->origin = TransCoords3To2(p->position);
				cps->start();
			}
			cps->update(dt);
		}

		p->life -= dt;

		if (p->life > 0)
		{
			// update acc
			p->speed[2] -= gravity * dt;
			float velocity = sqrt(p->speed[0] * p->speed[0] + p->speed[1] * p->speed[1] + p->speed[2] * p->speed[2]);
			for (int i = 0; i < 3; ++i) {
				p->speed[i] += p->linear_acc * p->speed[i] / velocity;
			}

			// update disturbance
			if (p->speed[2] != 0 && p->position[2] > 0) {
				float dis_dir3[3];
				dis_dir3[0] = p->dis_direction[0];
				dis_dir3[1] = p->dis_direction[1];
				dis_dir3[2] = - (p->speed[0] * dis_dir3[0] + p->speed[1] * dis_dir3[1]) / p->speed[2];
				float len = dis_dir3[0] * dis_dir3[0] + dis_dir3[1] * dis_dir3[1] + dis_dir3[2] * dis_dir3[2];
				len = sqrt(len);
				for (int i = 0; i < 3; ++i) {
					dis_dir3[i] /= len;
				}
				for (int i = 0; i < 3; ++i) {
					p->speed[i] += dis_dir3[i] * p->dis_speed;
				}

				p->dis_curr_len += p->dis_speed * dt;
				if (p->dis_curr_len > p->dis_region) {
					p->dis_direction[0] = -p->dis_direction[0];
					p->dis_direction[1] = -p->dis_direction[1];
					p->dis_curr_len = 0;
				}
			}

			// update angle
			if (orient_to_movement) 
			{
				d2d::Vector pos_old = TransCoords3To2(p->position);

				float _pos[3];
				for (int i = 0; i < 3; ++i) {
					_pos[i] = p->position[i] + p->speed[i] * dt;
				}
				d2d::Vector pos_new = TransCoords3To2(_pos);

				p->angle = d2d::Math::getLineAngle(pos_old, pos_new) - d2d::PI * 0.5f;
			} 
			else 
			{
		//		p->rotate -= inertia * dt;
//				if (p->rotate < 0) p->rotate = 0;
				if (p->position[2] > 0.1f)
					p->angle += p->rotate * dt;
			}

			// update position
			for (int i = 0; i < 3; ++i) {
				p->position[i] += p->speed[i] * dt;
			}
			if (p->position[2] < 0)
			{
				if (bounce)
				{
					p->position[2] *= -0.2f;
					p->speed[0] *= 0.4f;
					p->speed[1] *= 0.4f;
					p->speed[2] *= -0.2f;
				}
				else
				{
					p->speed[2] = 0;
					p->speed[0] = p->speed[1] = p->speed[2] = 0;
				}
			}

			p++;
		}
		else
		{
// 			// todo clear child's ps
// 			if (p->pc->m_bind_ps) {
// //				p->pc->m_bind_ps->reset();
// 				p->pc->m_bind_ps->stop();
// 			}

			remove(p);

			if (p >= pLast)
				return;
		}
	}
}

void ParticleSystem::SetDirection(float x, float y, float z)
{
	vec3 start(0, 0, 1), end(x, y, z);
	end.Normalize();
	SetDirection(Quaternion::CreateFromVectors(start, end));
}

void ParticleSystem::SetDirection(const Quaternion& dir)
{
	direction = dir.ToMatrix();
}

void ParticleSystem::start()
{
	active = true;
}

void ParticleSystem::stop()
{
	active = false;
	life = lifetime;
	emitCounter = 0;
}

void ParticleSystem::Clear()
{
	Particle* p = pStart;
	while (p != pLast)
	{
		remove(p);
		if (p >= pLast)
			return;
	}	
}

void ParticleSystem::reset()
{
//  	// todo clear child's ps
//  	Particle* p = pStart;
//  	while (p != pLast)
//  	{
//  		if (p->pc->m_bind_ps) {
//  			p->pc->m_bind_ps->reset();
// 			p->pc->m_bind_ps->stop();
//  		}
//  		++p;
//  	}

	pLast = pStart;
	life = lifetime = emission_time;
	emitCounter = 0;

	if (m_anim_recorder) {
		m_anim_recorder->Clear();
	}
	if (m_inv_record) {
		m_inv_record->Clear();
	}
}

void ParticleSystem::pause()
{
	active = false;
}

void ParticleSystem::SetLoop(bool loop)
{
	if (loop == m_loop) {
		return;
	}

	life = lifetime = emission_time;
	m_loop = loop;

	if (m_loop) {
		start();
	}
}

bool ParticleSystem::IsEmpty() const
{
	return pStart == pLast;
}

void ParticleSystem::ReloadTexture() const
{
	for (int i = 0, n = children.size(); i < n; ++i)
	{
		ParticleChild* child = children[i];
		child->symbol->ReloadTexture();
	}
}

void ParticleSystem::StoreAnimRecord(const std::string& filepath) const
{
	if (m_anim_recorder) {
		m_anim_recorder->StoreToFile(filepath);
	}
}

void ParticleSystem::StoreInvertRecord(const std::string& filepath) const
{
	if (m_inv_record) {
		m_inv_record->StoreToFile(filepath);
	}
}

void ParticleSystem::add()
{
	if (isFull()) return;

	pLast->pos = origin;

	std::vector<ParticleChild*> choices;
	for (size_t i = 0, n = children.size(); i < n; ++i)
		if (children[i]->symbol)
			choices.push_back(children[i]);
	if (choices.empty()) return;
	const int index = std::min((size_t) (choices.size() * d2d::Random::getNum0To1()), choices.size()-1);
	pLast->pc = choices[index];

	float min, max;

	min = min_life; max = max_life;
	pLast->life = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->lifetime = pLast->life;

	min = min_hori; max = max_hori;
	pLast->direction[0] = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->direction[0] *= d2d::TRANS_DEG_TO_RAD;

	min = min_vert; max = max_vert;
	pLast->direction[1] = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->direction[1] *= d2d::TRANS_DEG_TO_RAD;

	if (is_start_radius_3d) {
		TransCoords3D(start_radius, pLast->direction[0], pLast->direction[1], pLast->position);
	} else {
		TransCoords2D(start_radius, pLast->direction[0], pLast->position);
	}

	min = min_spd; max = max_spd;
	float speed = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	TransCoords3D(speed, pLast->direction[0], pLast->direction[1], pLast->speed);

	min = min_dis_region; max = max_dis_region;
	pLast->dis_region = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->dis_curr_len = 0;

	float dis_angle = rand() / (float(RAND_MAX)+1) * (d2d::PI * 2);
	pLast->dis_direction[0] = cos(dis_angle);
	pLast->dis_direction[1] = sin(dis_angle);

	min = min_dis_spd; max = max_dis_spd;
	pLast->dis_speed = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;

	min = min_linear_acc; max = max_linear_acc;
	float linear_acc = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->linear_acc = linear_acc;

	// 初始角度angle，每帧变换rotate
	//////////////////////////////////////////////////////////////////////////
	min = min_angular_spd; max = max_angular_spd;
	float angular_spd = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	angular_spd = angular_spd * d2d::TRANS_DEG_TO_RAD;
	pLast->rotate = angular_spd;

// 	pLast->angle = (rand() / (float(RAND_MAX)+1)) * d2d::PI * 2;
	//////////////////////////////////////////////////////////////////////////
	min = pLast->pc->min_rotate; max = pLast->pc->max_rotate;
	pLast->angle = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;

//	pLast->rotate = 0;
	//////////////////////////////////////////////////////////////////////////

	pLast->m_bind_time = 0;
	if (pLast->pc->bind_ps) {
//			// todo memory leak
//			delete pLast->m_bind_ps;
		pLast->m_bind_ps = pLast->pc->bind_ps->Clone();
	} else {
		delete pLast->m_bind_ps;
		pLast->m_bind_ps = NULL;
	}

	pLast++;
}

void ParticleSystem::remove(Particle * p)
{
	if (m_inv_record) {
		m_inv_record->AddItem(p);
	}
	if (!isEmpty()) {
		*p = *(--pLast);
	}
}

void ParticleSystem::TransCoords3D(float r, float hori, float vert, float result[3])
{
	float dxy = r * cos(vert);
	float dz = r * sin(vert);
	result[0] = dxy * cos(hori);
	result[1] = dxy * sin(hori);
	result[2] = dz;
}

void ParticleSystem::TransCoords2D(float r, float hori, float result[3])
{
//	float a = rand() / (float(RAND_MAX)+1) * (2 * d2d::PI);
	result[0] = r * cos(hori);
	result[1] = r * sin(hori);
	result[2] = 0;
}

}
