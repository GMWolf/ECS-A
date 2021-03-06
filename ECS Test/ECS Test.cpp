// ECS Test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "World.h"
#include "Vector3D.h"
#include "ComponentManager.h"
#include "Family.h"
#include "FamilySubscription.h"
#include "System.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <windows.h>


using namespace ecsa;

struct Position: public Vector3D{
	Position(float x, float y, float z) :Vector3D(x, y, z) {
	}
	Position(Vector3D v) : Vector3D(v.x, v.y, v.z) {
	}
	Position() :Vector3D(0, 0, 0) {
	}
};

struct Velocity: public Vector3D {
	Velocity(float x, float y, float z) :Vector3D(x, y, z) {
	}
	Velocity() :Vector3D() {

	}
};

struct Acceleration : public Vector3D {
	Acceleration(float x, float y, float z) :Vector3D(x, y, z) {
	}
	Acceleration() :Vector3D() {
	}
};

struct Age {
	Age() {};
	Age(float maxAge) : time(0), maxAge(maxAge){
	}
	float time;
	float maxAge;
};

struct Spawner {
	Spawner(float rate) : rate(rate), carry(0) {
	}
	Spawner() : rate(0) {}
	float rate;
	float carry;
};

struct Sparkling {
	Sparkling(float rate) : rate(rate), carry(0) {}
	Sparkling() : rate(0), carry(0) {}
	float rate;
	float carry;
};

struct Orbit {
	Orbit(Vector3D& around, float radius, float rate,  float t) : radius(radius), rate(rate), around(around), t(t) {
	}

	Orbit() : radius(0), rate(0), around(Vector3D(0, 0, 0)), t(0) {
	}

	float rate;
	Vector3D around;
	float radius;
	float t;
};


class PositionVelocitySystem : public ecsa::System {
public:

	PositionVelocitySystem(World& world) : positions(world.getComponentManager<Position>()),
		velocities(world.getComponentManager<Velocity>()),
		family(AllFamily::get<Position, Velocity>(world)),
		entities(world.getSubscription(family)),
		System(world) {
	};

	void initialize() override{

	};
	void update(float dt) override {
		for (int e : entities) {
			positions[e] += velocities[e] * dt;
		}
	};


private:
	ComponentManager<Position>& positions;
	ComponentManager<Velocity>& velocities;
	AllFamily family;
	FamilySubscription& entities;
};

class VelocityAccelerationSystem : public ecsa::System {
public:

	VelocityAccelerationSystem(World& world) : accelerations(world.getComponentManager<Acceleration>()),
		velocities(world.getComponentManager<Velocity>()),
		family(AllFamily::get<Acceleration, Velocity>(world)),
		entities(world.getSubscription(family)),
		System(world) {
	};

	void initialize() override {
	};
	void update(float dt) override {
		for (int e : entities) {
			velocities[e] += accelerations[e] * dt;
		}
	};
private:

	ComponentManager<Acceleration>& accelerations;
	ComponentManager<Velocity>& velocities;
	AllFamily family;
	FamilySubscription& entities;
};

class AgeSystem : public ecsa::System {
public:

	AgeSystem(ecsa::World& world) : ages(world.getComponentManager<Age>()),
		family(ecsa::AllFamily::get<Age>(world)),
		entities(world.getSubscription(family)),
		System(world) {
	};

	void initialize() override {
	};
	void update(float dt) override{
		for (int e : entities) {
			Age& age = ages[e];
			age.time += dt;
			if (age.time >= age.maxAge) {
				world.destroy(e);
			}
		}
	};
private:

	ecsa::ComponentManager<Age>& ages;
	ecsa::AllFamily family;
	ecsa::FamilySubscription& entities;
};

class SpawnerSystem : public ecsa::System {
public:
	SpawnerSystem(World& world) : positions(world.getComponentManager<Position>()),
		spawners(world.getComponentManager<Spawner>()),
		velocities(world.getComponentManager<Velocity>()),
		accelerations(world.getComponentManager<Acceleration>()),
		ages(world.getComponentManager<Age>()),
		sparklings(world.getComponentManager<Sparkling>()),
		family(AllFamily::get<Spawner, Position>(world)),
		entities(world.getSubscription(family)),
		System(world) {

	};

	void initialize() override {};

	void update(float dt) override {
		for (int e : entities) {
			Spawner& spawn = spawners[e];
			for (spawn.carry += spawn.rate * dt; spawn.carry >= 1; spawn.carry -= 1) {
				int part = world.create();
				positions.create(part);
				velocities.create(part, (10 * rand() / (float)RAND_MAX) - 5, (8 * rand() / (float)RAND_MAX)+5, (10 * rand() / (float)RAND_MAX) - 5);
				accelerations.create(part, 0, -1, 0);
				ages.create(part, 10);
				sparklings.create(part, 100);
			}

		}
	};

private:
	ComponentManager<Position>& positions;
	ComponentManager<Spawner>& spawners;
	ComponentManager<Velocity>& velocities;
	ComponentManager<Acceleration>& accelerations;
	ComponentManager<Age>& ages;
	ComponentManager<Sparkling>& sparklings;
	AllFamily family;
	FamilySubscription& entities;
};

class SparklingSystem : public ecsa::System {
public:
	SparklingSystem(ecsa::World& world) : positions(world.getComponentManager<Position>()),
		sparklings(world.getComponentManager<Sparkling>()),
		velocities(world.getComponentManager<Velocity>()),
		accelerations(world.getComponentManager<Acceleration>()),
		ages(world.getComponentManager<Age>()),
		family(ecsa::AllFamily::get<Sparkling, Position>(world)),
		entities(world.getSubscription(family)),
		System(world) {
	};

	void initialize() override {};

	void update(float dt) override {
		for (int e : entities) {
			Sparkling& sparks = sparklings[e];
			for (sparks.carry += sparks.rate*dt; sparks.carry >= 1; sparks.carry -= 1) {
				int spark = world.create();
				positions.create(spark, positions[e]);
				velocities.create(spark, (10 * rand() / (float)RAND_MAX) - 5, (10 * rand() / (float)RAND_MAX) - 5, (10 * rand() / (float)RAND_MAX) - 5);
				ages.create(spark, 1);
			}

		}
	};

private:
	ComponentManager<Position>& positions;
	ComponentManager<Sparkling>& sparklings;
	ComponentManager<Velocity>& velocities;
	ComponentManager<Acceleration>& accelerations;
	ComponentManager<Age>& ages;
	AllFamily family;
	FamilySubscription& entities;
};

class OrbitSystem : public ecsa::System {
public:
	OrbitSystem(World& world) : positions(world.getComponentManager<Position>()),
		orbits(world.getComponentManager<Orbit>()),
		family(AllFamily::get<Position, Orbit>(world)),
		entities(world.getSubscription(family)),
		System(world){
	}

	void initialize() override {
	}

	void update(float dt) override {
		for (int e : entities) {
			Position& pos = positions[e];
			Orbit& orbit = orbits[e];
			
			orbit.t += (dt * orbit.rate);
			
			pos.x = orbit.around.x + cos(orbit.t * 3.14) * orbit.radius;
			pos.z = orbit.around.z + sin(orbit.t *3.14) * orbit.radius;
			pos.y = orbit.around.y;
		}
	}

private:
	ComponentManager<Position>& positions;
	ComponentManager<Orbit>& orbits;
	AllFamily family;
	FamilySubscription& entities;
};

class OutputSystem : public ecsa::System {
public:
	OutputSystem(World& world) : positions(world.getComponentManager<Position>()),
		family(AllFamily::get<Position>(world)),
		entities(world.getSubscription(family)),
		System(world) {
	};

	void initialize() override {
		i = 0;
	};

	void update(float dt) override {
		std::ofstream file;
		file.open("output\\positions_" + std::to_string(i) + ".csv");
		file << "x, y, z, e" << std::endl;
		for (int e : entities) {
			Position& pos = positions[e];
			file << pos.x << "," << pos.y << "," << pos.z << "," << e << "," << std::endl;
			//std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;
		}
		file.close();
		i++;
	};

private:
	ComponentManager<Position>& positions;
	AllFamily family;
	FamilySubscription& entities;
	int i;
};





using namespace std::chrono;
int main()
{
	World world;

	world.with<PositionVelocitySystem>();
	world.with<VelocityAccelerationSystem>();
	world.with<AgeSystem>();
	world.with<SpawnerSystem>();
	world.with<SparklingSystem>();
	//world.with<OrbitSystem>();
	//world.with<OutputSystem>();

	ComponentManager<Position>& positions = world.getComponentManager<Position>();
	ComponentManager<Spawner>& spawners = world.getComponentManager<Spawner>();
	ComponentManager<Orbit>& orbits = world.getComponentManager<Orbit>();
	AllFamily fam = AllFamily::get<Position>(world);
	FamilySubscription& posSub = world.getSubscription(fam);
	world.initialize();
	
	//Instantiate spawner
	int spawner = world.create();
	spawners.create(spawner, 1);
	positions.create(spawner);

	world.update(0.1);
	for (int e : posSub) {
		std::cout << e << std::endl;
	}
	
	//create a bunch of spinners
	/*for (int i = 0; i < 2000; i++) {
		int e = world.create();
		positions.create(e, 0, 0, 0);
		orbits.create(e, Vector3D(0, 0, 0), 10 + 5 * (rand() / (float)RAND_MAX),  (rand() / (float)RAND_MAX), rand() / (float)RAND_MAX);
	}*/

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	//simulate
	const int itCount = 80000;
	for (int i = 0; i < itCount; i++) {
		world.update(0.1);
	}
	
	
	
	

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	std::cout << duration << std::endl;

	getchar();
}

