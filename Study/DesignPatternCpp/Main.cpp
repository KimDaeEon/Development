#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <random>

#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>

using namespace std;
#include "GOF.h"

int main()
{
	// Creational
	//GOF::Creational::AbstractFactory::Test();
	//GOF::Creational::FactoryMethod::UsingInheritance::Test();
	//GOF::Creational::FactoryMethod::UsingJustMethod::Test();
	//GOF::Creational::ProtoType::Test();
	//GOF::Creational::Builder::Test();
	//GOF::Creational::Singleton::Test();


	
	// Structural
	//GOF::Structural::Adapter::Test();
	//GOF::Structural::Bridge::Test();
	//GOF::Structural::Composite::Test();
	//GOF::Structural::Decorator::Test();
	//GOF::Structural::Facade::Test();
	//GOF::Structural::FlyWeight::Test();
	//GOF::Structural::Proxy::Test();
	


	// Behavioural
	//GOF::Behavioural::ChainOfResponsibility::Test();
	//GOF::Behavioural::Command::Test();
	//GOF::Behavioural::Interpreter::Test();
	//GOF::Behavioural::Iterator::Test();
	//GOF::Behavioural::Mediator::Test();
	//GOF::Behavioural::Memento::Test();
	//GOF::Behavioural::Observer::Test();
	//GOF::Behavioural::State::Test();
	//GOF::Behavioural::Strategy::Test();
	//GOF::Behavioural::TemplateMethod::Test();
	GOF::Behavioural::Visitor::Test();

	return 1;
}	