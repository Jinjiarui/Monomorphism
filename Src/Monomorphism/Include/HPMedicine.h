#ifndef __HPMedicine_H__
#define __HPMedicine_H__

#include "Include\Item.h"
#include <OWE.h>
#include <Texture2D.h>

class HPMedicine: public Item
{
public:
	double StayTime;
	HPMedicine(double _StayTime, bool _Dead, bool _Broken, glm::vec2 _lp, glm::vec2 _rp, OWE::Texture2DView & t);
	// Dead��ʾ���е����Ա�ʾװ�����Broken��ʾStayTime=0
	bool Dead;
	bool Broken;
	glm::vec2 lp;
	glm::vec2 rp;
	OWE::Texture2DView _texture;
	void SetPosition(glm::vec2 _lp, glm::vec2 _rp);
	void Update(double);
	void Draw(const OWE::ScreenScale &screenscale);
	std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;
	void InteractWithActor(Actor *actor);

    bool IsDead(void) const;
};

#endif //_HPMedicine_