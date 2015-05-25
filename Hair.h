#ifndef HAIR_H
#define HAIR_H

#include <math.h>

class Hair : public Test
{
public:
	
	//Consturctor
	Hair()
	{

		//Ground creation
		b2Body* ground = NULL;
		{
			b2BodyDef bd; //create a body definition for the ground

			bd.position.Set(0.0f, 0.0f);
			ground = m_world->CreateBody(&bd);

			
		}

		//head
		{
			//Head body
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0,0);
			b2Body* head = m_world->CreateBody(&bd);

			//Head shape
			b2CircleShape shape;
			shape.m_radius = 5.0f;
			head->CreateFixture(&shape, 20.0f);
		
			//The joint the head will rotate with
			b2RevoluteJoint* m_joint;
			
			b2RevoluteJointDef revoluteJointDef;
			revoluteJointDef.bodyA = head;
			revoluteJointDef.bodyB = ground;
			revoluteJointDef.collideConnected = false;
			revoluteJointDef.localAnchorB.Set(0,0);//center of the circle
			m_joint = (b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDef );



	

		//Strand of hair creation
			int32 N = 50;//number of hairs on the head
			for(int i = 1; i<= N; i++){
				//i will count for every degree
				float x = shape.m_radius*cos(float(i))+bd.position.x;
				float y = shape.m_radius*sin(float(i))+bd.position.y;
				Strand(head,10,x,y);
			}
		}

		{
			m_ropeDef.bodyA = ground;
			m_rope = m_world->CreateJoint(&m_ropeDef);
		}
	}

	//create a single strand of hair
	void Strand(b2Body* ground, int32 length, float Xpos, float Ypos){

		b2PolygonShape shape; //create a polygon shape
			shape.SetAsBox(0.5f, 0.01f);//use the shape to create a box, to function as a segment of hair

			b2FixtureDef fd; //Define the fixture
			fd.shape = &shape; //set the shape of the fixture to the box that shape was set to
			fd.density = 20.0f; //set the segemnt density
			fd.friction = 0.2f;
			fd.filter.categoryBits = 0x0001;
			fd.filter.maskBits = 0xFFFF & ~0x0002;

			b2RevoluteJointDef jd; //Create the joint definition
			jd.collideConnected = false;

			jd.lowerAngle = -45 * ((22/7) / 180) ;
			jd.upperAngle =  45 * ((22/7) / 180) ;
			fd.restitution = 0.01f;
			

			const int32 N = length;//the maximum amount of segments
			const float32 y = Ypos;
			const float32 x = Xpos;
			//m_ropeDef.localAnchorA.Set(x, y);//set the anchor point that the segments will rotate around

			b2Body* prevBody = ground;

			for (int32 i = 0; i < N; ++i)
			{
				b2BodyDef bd;// define a body
				bd.type = b2_dynamicBody; // make the body a dynamic body
				bd.position.Set((0.5f + 1.0f * i)+x, y); // set its postion at the end of the previous segment

				if (i == N - 1)//if current segment is the last segment
				{
					shape.SetAsBox(0.1f, 0.1f); //set the end point as a smaller square
					fd.density = 100.0f;	//give the point a higer density to weigh it down
					fd.filter.categoryBits = 0x0002;
					bd.position.Set((1.0f * i)+x, y); // set its postion at the end of the previous segment
					bd.angularDamping = 0.4f;
				}

				b2Body* body = m_world->CreateBody(&bd);

				body->CreateFixture(&fd);

				b2Vec2 anchor(float32(i)+x, y);// Anchor for the root of the segment
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);//joint the segment to the previous

				prevBody = body;
			}


			m_ropeDef.localAnchorB.SetZero();//offset last segment anchor so it appears at the center of the segment

			float32 extraLength = 0.01f;
			m_ropeDef.maxLength = N - 1.0f + extraLength;
			m_ropeDef.bodyB = prevBody;
	}


	void Keyboard(unsigned char key)
	{
		switch (key)
		{
			
		}

	}

	void Step(Settings* settings)
	{
		
		Test::Step(settings);//Draw and update?
	
	}

	static Test* Create()
	{
		return new Hair;
	}

	
	b2RopeJointDef m_ropeDef;
	b2Joint* m_rope;
};

#endif