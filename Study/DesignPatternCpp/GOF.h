#pragma once
namespace GOF
{
	namespace Granularity
	{
		// 세분화 정도가 커서 기능별로 함수가 나뉘어져 있다.
		class BigGranularity
		{
			static void Update()
			{
				UpdatePhysics();
				UpdateAnimations();
				UpdateAI();
			}

			static void UpdateAnimations()
			{
				cout << "Udpate Animations" << endl;
			}

			static void UpdateAI()
			{
				cout << "Udpate AI" << endl;
			}

			static void UpdatePhysics()
			{
				cout << "Udpate Physics" << endl;
			}
		};

		// 세분화 정도가 작아서 기능이 큰 덩어리로 작동한다. 함수 호출은 더 빨라진다.
		class SmallGranularity
		{
			static void Update()
			{
				cout << "Udpate Physics" << endl;
				cout << "Udpate Animations" << endl;
				cout << "Udpate AI" << endl;
			}
		};
	}


	// 객체 생성 과정을 추상화시킨 패턴들
	namespace Creational
	{
		namespace AbstractFactory
		{
			// 만들려는 클래스 군집을 팩토리 추상 클래스의 구현체로서 제공하는 것.
			// 이 때에 팩토리가 생성하는 객체도 추상화된 클래스로 리턴한다.

			// 1. 엔티티 및 무기 인터페이스
			class Entity
			{
			public:
				virtual void Display() = 0;
				virtual ~Entity() = default;
			};

			class Weapon
			{
			public:
				virtual void Use() = 0;
				virtual ~Weapon() = default;
			};

			// 2. 몬스터 & 플레이어 (엔티티)
			class Monster : public Entity
			{
			public:
				void Display() override { std::cout << "Monster Created!\n"; }
			};

			class Player : public Entity
			{
			public:
				void Display() override { std::cout << "Player Created!\n"; }
			};

			// 3. 무기 종류
			class Sword : public Weapon
			{
			public:
				void Use() override { std::cout << "Swinging a sword!\n"; }
			};

			class Bow : public Weapon
			{
			public:
				void Use() override { std::cout << "Shooting an arrow!\n"; }
			};

			// 4. 추상 팩토리 인터페이스 (팩토리 메서드 포함)
			class GameFactory
			{
			public:
				// 이처럼 여러 개의 FactoryMethod 를 포함해서 하나의 기능과 관련된 군집 클래스들의 생성을 책임진다.
				virtual std::shared_ptr<Entity> CreateCharacter() = 0;
				virtual std::shared_ptr<Weapon> CreateWeapon() = 0;
				virtual ~GameFactory() = default;
			};

			// 5. 판타지 게임 팩토리 (추상 팩토리 구현)
			class CharacterGameFactory : public GameFactory
			{
			public:
				std::shared_ptr<Entity> CreateCharacter() override
				{
					return std::make_shared<Player>();
				}

				std::shared_ptr<Weapon> CreateWeapon() override
				{
					return std::make_shared<Sword>();
				}
			};

			// 6. 몬스터 게임 팩토리 (추상 팩토리 구현)
			class MonsterGameFactory : public GameFactory
			{
			public:
				std::shared_ptr<Entity> CreateCharacter() override
				{
					return std::make_shared<Monster>();
				}

				std::shared_ptr<Weapon> CreateWeapon() override
				{
					return std::make_shared<Bow>();
				}
			};

			// 7. 테스트 코드
			void Test()
			{
				std::shared_ptr<GameFactory> factory;

				// 판타지 게임 팩토리
				factory = std::make_shared<CharacterGameFactory>();
				std::shared_ptr<Entity> player = factory->CreateCharacter();
				std::shared_ptr<Weapon> sword = factory->CreateWeapon();
				player->Display();  // "Player Created!"
				sword->Use();       // "Swinging a sword!"

				// 몬스터 게임 팩토리
				factory = std::make_shared<MonsterGameFactory>();
				std::shared_ptr<Entity> monster = factory->CreateCharacter();
				std::shared_ptr<Weapon> bow = factory->CreateWeapon();
				monster->Display();  // "Monster Created!"
				bow->Use();          // "Shooting an arrow!"
			}
		}

		namespace FactoryMethod
		{
			// 객체 생성을 하위 클래스에서 담당하도록 하여, 객체 생성 코드와 객체 사용 코드를 분리하는 패턴.
			// 기본적으로 인터페이스나 추상 클래스를 정의하고, 실제 객체 생성은 이를 상속한 하위 클래스에서 수행함.
			// 이 패턴을 사용하면 클라이언트 코드가 구체적인 클래스의 인스턴스를 직접 생성하지 않고, 인터페이스를 통해 객체를 생성할 수 있도록 유도함.
			// 꼭 상속을 통하여 하지 않아도 특정 메서드가 다른 클래스의 객체 생성을 메서드 내부에서 캡슐화 한다면 넓게 봤을 때에 이 패턴으로 볼 수 있다고 한다.

			namespace UsingInheritance
			{
				class Entity
				{
				public:
					virtual ~Entity() = default;
					virtual void Attack() const = 0;

					// 팩토리 메서드 (상속을 통해 객체 생성 책임 위임)
					virtual std::unique_ptr<Entity> Create() const = 0;
				};

				// 몬스터 클래스
				class Monster : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "몬스터가 공격합니다!\n";
					}

					std::unique_ptr<Entity> Create() const override
					{
						return std::make_unique<Monster>();
					}
				};

				// 플레이어 클래스
				class Player : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "플레이어가 공격합니다!\n";
					}

					std::unique_ptr<Entity> Create() const override
					{
						return std::make_unique<Player>();
					}
				};

				// MyPlayer 클래스 (Player 상속)
				class MyPlayer : public Player
				{
				public:
					void Attack() const override
					{
						std::cout << "나만의 캐릭터가 공격합니다!\n";
					}

					std::unique_ptr<Entity> Create() const override
					{
						return std::make_unique<MyPlayer>();
					}
				};

				void Test()
				{
					std::unique_ptr<Entity> monster = std::make_unique<Monster>()->Create();
					std::unique_ptr<Entity> player = std::make_unique<Player>()->Create();
					std::unique_ptr<Entity> myPlayer = std::make_unique<MyPlayer>()->Create();

					monster->Attack();
					player->Attack();
					myPlayer->Attack();
				}
			}

			namespace UsingJustMethod
			{
				// 기본 엔티티 클래스
				class Entity
				{
				public:
					virtual ~Entity() = default;
					virtual void Attack() const = 0;

					// 팩토리 메서드 (문자열을 이용한 객체 생성)
					static std::unique_ptr<Entity> Create(const std::string& type);
				};

				// 몬스터 클래스
				class Monster : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "몬스터가 공격합니다!\n";
					}
				};

				// 플레이어 클래스
				class Player : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "플레이어가 공격합니다!\n";
					}
				};

				// MyPlayer 클래스 (Player 상속)
				class MyPlayer : public Player
				{
				public:
					void Attack() const override
					{
						std::cout << "나만의 캐릭터가 공격합니다!\n";
					}
				};

				// 팩토리 메서드 구현
				std::unique_ptr<Entity> Entity::Create(const std::string& type)
				{
					if (type == "Monster")
					{
						return std::make_unique<Monster>();
					}
					else if (type == "Player")
					{
						return std::make_unique<Player>();
					}
					else if (type == "MyPlayer")
					{
						return std::make_unique<MyPlayer>();
					}
					return nullptr; // 잘못된 타입 처리
				}

				// 실행 코드
				void Test()
				{
					auto monster = Entity::Create("Monster");
					auto player = Entity::Create("Player");
					auto myPlayer = Entity::Create("MyPlayer");

					if (monster)
					{
						monster->Attack();
					}
					if (player)
					{
						player->Attack();
					}
					if (myPlayer)
					{
						myPlayer->Attack();
					}
				}
			}
		}

		namespace ProtoType
		{
			// Clone 이라는 인터페이스를 구현하도록 강제한다. 이를 통해 상속 관계에서 다형성을 활용하여 복제본을 올바르게 생성할 수 있도록 한다.
			// 이미 존재하는 객체를 복제하여 새로운 객체를 생성하고 싶을 때, 동일한 속성을 가진 객체를 여러개 만들고 싶을 때에 사용할 수 있다.
			// 팩토리 메서드나 추상 팩토리 패턴에 도움을 많이 받을 수 있으며, 구성과 데코레이터 패턴에서 프로토타입 쓰면 좋은 경우가 많다.

		// 1. Clone 인터페이스
			class Entity
			{
			public:
				virtual std::shared_ptr<Entity> Clone() = 0;
				virtual void Display() = 0;
				virtual ~Entity() = default;
			};

			// 2. 컴포넌트 (구성 요소) - 동적으로 특성 부여 가능
			class Component
			{
			public:
				virtual void ApplyEffect() = 0;
				virtual ~Component() = default;
			};

			class AComponent : public Component
			{
			public:
				void ApplyEffect() override
				{
					std::cout << "A 컴포넌트 효과 적용\n";
				}
			};

			class BComponent : public Component
			{
			public:
				void ApplyEffect() override
				{
					std::cout << "B 컴포넌트 효고 적용\n";
				}
			};

			// 3. Character 클래스 (컴포넌트 기반 구성 적용)
			class Character : public Entity
			{
			private:
				std::vector<std::shared_ptr<Component>> components;

			public:
				Character() = default;

				// 복사 생성자 (컴포넌트도 복사)
				Character(const Character& other)
				{
					for (const auto& comp : other.components)
					{
						components.push_back(comp); // 얕은 복사 (공유)
					}
				}

				std::shared_ptr<Entity> Clone() override
				{
					// 복사 생성자를 통해서 Clone 기능 작동
					return std::make_shared<Character>(*this);
				}

				void AddComponent(std::shared_ptr<Component> component)
				{
					components.push_back(std::move(component));
				}

				void ApplyComponents()
				{
					for (const auto& component : components)
					{
						component->ApplyEffect();
					}
				}

				void Display() override
				{
					std::cout << "Character Created!\n";
					ApplyComponents();
				}
			};

			// 4. 데코레이터 패턴 - 캐릭터에 추가 기능 부여
			// 중첩적으로 어떤 기능을 추가하거나, 내가 고칠 수 없는 코드에 무언가 기능을 더하고 싶을 때에 쓰면 좋다. 
			// 구체적인 예시는 아래에 데코레이터 패턴에서 참고

			class CharacterDecorator : public Character
			{
			private:
				std::shared_ptr<Character> baseCharacter;
				std::string extraFeature;

			public:
				CharacterDecorator(std::shared_ptr<Character> base, std::string feature)
					: baseCharacter(std::move(base)), extraFeature(std::move(feature))
				{
				}

				std::shared_ptr<Entity> Clone() override
				{
					return std::make_shared<CharacterDecorator>(static_pointer_cast<Character>(baseCharacter->Clone()), extraFeature);
				}

				void Display() override
				{
					baseCharacter->Display();
					std::cout << "추가 특성: " << extraFeature << "\n";
				}
			};

			// 5. 프로토타입 팩토리 (팩토리 메서드 적용)
			// 팩토리 관련 클래스가 많이 필요해진다면 추상 팩토리도 아래와 같이 prototypes 맵을 각각 클래스별로 다르게 유지하도록 하여 구현 가능하다.
			class EntityFactory
			{
			private:
				std::unordered_map<std::string, std::shared_ptr<Entity>> prototypes;

			public:
				void RegisterPrototype(const std::string& key, std::shared_ptr<Entity> prototype)
				{
					prototypes[key] = prototype;
				}

				// 팩토리 메서드 내부를 ProtoType 함수를 사용하여 구현
				std::shared_ptr<Entity> CreateEntity(const std::string& key)
				{
					if (prototypes.find(key) != prototypes.end())
					{
						return prototypes[key]->Clone();
					}
					return nullptr;
				}
			};

			// 6. 테스트 코드
			static void Test()
			{
				EntityFactory factory;

				// 기본 캐릭터 등록 (프로토타입)
				std::shared_ptr<Character> baseCharacter = std::make_shared<Character>();
				factory.RegisterPrototype("BasicCharacter", baseCharacter);

				// 캐릭터 복제 후 컴포넌트 추가 (구성 패턴 적용)
				std::shared_ptr<Entity> char1 = factory.CreateEntity("BasicCharacter");
				std::shared_ptr<Character> strongChar = std::dynamic_pointer_cast<Character>(char1);
				if (strongChar)
				{
					strongChar->AddComponent(std::make_shared<AComponent>());
					strongChar->Display();
				}

				std::shared_ptr<Entity> char2 = factory.CreateEntity("BasicCharacter");
				std::shared_ptr<Character> fastChar = std::dynamic_pointer_cast<Character>(char2);
				if (fastChar)
				{
					fastChar->AddComponent(std::make_shared<BComponent>());
					fastChar->Display();
				}

				// 데코레이터 적용 (추가 기능 부여)
				std::shared_ptr<Entity> armoredChar = std::make_shared<CharacterDecorator>(strongChar, "Armor Boost");
				armoredChar->Display();
			}
		}

		namespace Builder
		{
			// 객체의 생성 과정과 표현 방식(최종적으로 생성된 모습)을 분리하여 동일한 생성 절차에서 서로 다른 표현의 객체를 만들 수 있도록 하는 패턴.
			// 복잡한 객체를 단계적으로 구성할 때 유용하며, 생성자에 많은 매개변수를 전달하는 문제를 해결하는 데 도움을 줌.
			// 디렉터(Director)가 빌더(Builder) 인터페이스를 사용하여 객체를 조립하고, 클라이언트는 빌더를 통해 필요한 객체를 생성할 수 있음.

			// Monster 클래스 (생성될 객체)
			class Monster
			{
			private:
				std::string name;
				int hp;
				int attackPower;
				std::string specialAbility;

			public:
				void SetName(const std::string& n) { name = n; }
				void SetHP(int h) { hp = h; }
				void SetAttackPower(int atk) { attackPower = atk; }
				void SetSpecialAbility(const std::string& ability) { specialAbility = ability; }

				void Show() const
				{
					std::cout << "몬스터 정보: " << name << "\n"
						<< "체력: " << hp << "\n"
						<< "공격력: " << attackPower << "\n"
						<< "특수 능력: " << specialAbility << "\n"
						<< "----------------------\n";
				}
			};

			// MonsterBuilder 인터페이스
			class IMonsterBuilder
			{
			public:
				virtual ~IMonsterBuilder() = default;
				virtual void BuildName() = 0;
				virtual void BuildHP() = 0;
				virtual void BuildAttackPower() = 0;
				virtual void BuildSpecialAbility() = 0;
				virtual std::shared_ptr<Monster> GetResult() = 0;
			};

			// Concrete Builder - 보스 몬스터 생성
			class BossMonsterBuilder : public IMonsterBuilder
			{
			private:
				std::shared_ptr<Monster> monster;

			public:
				BossMonsterBuilder() { Reset(); }

				void Reset() { monster = std::make_shared<Monster>(); }

				void BuildName() override { monster->SetName("어둠의 군주"); }
				void BuildHP() override { monster->SetHP(5000); }
				void BuildAttackPower() override { monster->SetAttackPower(250); }
				void BuildSpecialAbility() override { monster->SetSpecialAbility("광역 파괴 스킬"); }

				std::shared_ptr<Monster> GetResult() override
				{
					auto result = monster;
					Reset(); // 다음 객체 생성을 위해 리셋
					return result;
				}
			};

			// Concrete Builder - 일반 몬스터 생성
			class NormalMonsterBuilder : public IMonsterBuilder
			{
			private:
				std::shared_ptr<Monster> monster;

			public:
				NormalMonsterBuilder() { Reset(); }

				void Reset() { monster = std::make_shared<Monster>(); }

				void BuildName() override { monster->SetName("슬라임"); }
				void BuildHP() override { monster->SetHP(100); }
				void BuildAttackPower() override { monster->SetAttackPower(10); }
				void BuildSpecialAbility() override { monster->SetSpecialAbility("점프 공격"); }

				std::shared_ptr<Monster> GetResult() override
				{
					auto result = monster;
					Reset(); // 다음 객체 생성을 위해 리셋
					return result;
				}
			};

			// Director - 몬스터 생성 조립 관리자
			class MonsterDirector
			{
			private:
				std::shared_ptr<IMonsterBuilder> builder;

			public:
				void SetBuilder(std::shared_ptr<IMonsterBuilder> b) { builder = std::move(b); }

				void ConstructBasicMonster()
				{
					if (builder)
					{
						builder->BuildName();
						builder->BuildHP();
					}
				}

				void ConstructFullMonster()
				{
					if (builder)
					{
						builder->BuildName();
						builder->BuildHP();
						builder->BuildAttackPower();
						builder->BuildSpecialAbility();
					}
				}
			};

			// Test 함수 (메인 로직 실행)
			void Test()
			{
				std::shared_ptr<BossMonsterBuilder> bossBuilder = std::make_shared<BossMonsterBuilder>();
				std::shared_ptr<NormalMonsterBuilder> normalBuilder = std::make_shared<NormalMonsterBuilder>();
				MonsterDirector director;

				std::cout << "일반 몬스터 생성 (기본 속성만):\n";
				director.SetBuilder(normalBuilder);
				director.ConstructBasicMonster();
				std::shared_ptr<Monster> normalMonster = normalBuilder->GetResult();
				normalMonster->Show();

				std::cout << "보스 몬스터 생성 (풀 세팅):\n";
				director.SetBuilder(bossBuilder);
				director.ConstructFullMonster();
				std::shared_ptr<Monster> bossMonster = bossBuilder->GetResult();
				bossMonster->Show();
			}
		}

		namespace Singleton
		{
			// 싱글턴 템플릿 클래스 (상속 지원)
			template <typename T>
			class Singleton
			{
			protected:
				Singleton() {}

			public:
				// 복사 및 할당을 못하도록 막기
				Singleton(const Singleton&) = delete;
				Singleton& operator=(const Singleton&) = delete;

				static T& GetInstance()
				{
					static T instance;
					return instance;
				}
			};

			// 자식 클래스 1
			class GameManager : public Singleton<GameManager>
			{
				friend class Singleton<GameManager>; // Singleton에서 private 생성자 접근 가능

			private:
				GameManager() {}

			public:
				void Show()
				{
					std::cout << "GameManager 인스턴스 실행됨" << std::endl;
				}
			};

			// 자식 클래스 2
			class AudioManager : public Singleton<AudioManager>
			{
				friend class Singleton<AudioManager>;

			private:
				AudioManager() {}

			public:
				void PlaySound()
				{
					std::cout << "AudioManager에서 사운드 재생" << std::endl;
				}
			};

			// 테스트 함수
			void Test()
			{
				GameManager& game = GameManager::GetInstance();
				game.Show();

				AudioManager& audio = AudioManager::GetInstance();
				audio.PlaySound();

				// 동일한 인스턴스 확인
				GameManager& game2 = GameManager::GetInstance();
				if (&game == &game2)
				{
					std::cout << "GameManager는 단 하나의 인스턴스만 존재함" << std::endl;
				}
			}

		}
	}


	// 여러 클래스들의 객체를 합쳐서 특정 구조를 만드는 것과 관련된 패턴
	namespace Structural
	{
		namespace Adapter
		{
			// 어떤 클래스의 인터페이스가 현재 시스템과 맞지 않을 때에 이를 맞추기 위해 사용한다.
			// 현재 시스템에서 사용하는 인터페이스를 상속받은 클래스에 계속 사용해야 하는 클래스 객체를 상속이나 구성을 사용하여 인터페이스를 맞춘다.
			// Cpp에서는 다중 상속이 가능해서 인터페이스는 public 상속을, 타겟 기능은 private 상속을 통해 이를 구현한다.
			// 다른 언어에서는 다중 상속이 안되어서 타겟 객체를 참조하고 인터페이스만 상속받아서 구현한다.
		
			// 새로운 게임 시스템에서 사용하는 길찾기 인터페이스
			class IPathfinding
			{
			public:
				virtual std::vector<std::pair<int, int>> FindPath(int startX, int startY, int endX, int endY) = 0;
				virtual ~IPathfinding() = default;
			};

			// 기존 길찾기 시스템 (인터페이스가 다름)
			class OldPathfindingSystem
			{
			public:
				void ComputePath(int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>>& path)
				{
					std::cout << "OldPathfindingSystem: 길찾기 실행 (" << startX << "," << startY << ") -> (" << endX << "," << endY << ")\n";

					// 예제: 단순한 직선 경로 (실제 게임에서는 A* 알고리즘 등 사용)
					int dx = (endX > startX) ? 1 : -1;
					int dy = (endY > startY) ? 1 : -1;

					int x = startX, y = startY;
					while (x != endX || y != endY)
					{
						if (x != endX) x += dx;
						if (y != endY) y += dy;
						path.emplace_back(x, y);
					}
				}
			};

			// 어댑터 클래스 (새로운 길찾기 인터페이스를 구현하면서 기존 시스템을 내부에서 사용)
			class PathfindingAdapter : public IPathfinding
			{
			private:
				std::shared_ptr<OldPathfindingSystem> oldPathfinding;

			public:
				PathfindingAdapter(std::shared_ptr<OldPathfindingSystem> oldSystem) : oldPathfinding(oldSystem) {}

				std::vector<std::pair<int, int>> FindPath(int startX, int startY, int endX, int endY) override
				{
					std::vector<std::pair<int, int>> path;
					std::cout << "PathfindingAdapter: 변환 후 호출 -> ";
					oldPathfinding->ComputePath(startX, startY, endX, endY, path);
					return path;
				}
			};

			// 게임 캐릭터 클래스 (새로운 길찾기 시스템을 사용)
			class GameCharacter
			{
			private:
				std::shared_ptr<IPathfinding> pathfinder;

			public:
				GameCharacter(std::shared_ptr<IPathfinding> pathfindingSystem) : pathfinder(pathfindingSystem) {}

				void MoveTo(int x, int y)
				{
					std::cout << "GameCharacter: 이동 요청 -> (" << x << ", " << y << ")\n";
					std::vector<std::pair<int, int>> path = pathfinder->FindPath(0, 0, x, y);

					std::cout << "경로: ";
					for (auto& p : path)
					{
						std::cout << "(" << p.first << ", " << p.second << ") ";
					}
					std::cout << "\n";
				}
			};

			// 테스트 함수
			static void Test()
			{
				std::shared_ptr<OldPathfindingSystem> oldPathfinder = std::make_shared<OldPathfindingSystem>();
				std::shared_ptr<IPathfinding> adapter = std::make_shared<PathfindingAdapter>(oldPathfinder);

				GameCharacter player(adapter);

				player.MoveTo(3, 4);
				player.MoveTo(-2, -1);
			}
		}

		namespace Bridge
		{
			// 추상 클래스 안의 가상 함수 기능을, 다른 인터페이스를 통해 작동하도록 하여 추상 클래스의 변화와 인터페이스의 변화가 독립적으로 이루어질 수 있도록 만든 패턴.
			// 구현한 것만 보면 어댑터와 유사한데, 어댑터 패턴은 인터페이스를 포함하는 어댑터 클래스가 변화를 위한 클래스는 아니라는 점에서 다르다.

			class IAIBehavior
			{
			public:
				virtual void ExecuteAI() = 0;
				virtual ~IAIBehavior() = default;
			};

			// 공격적인 AI
			class AggressiveAI : public IAIBehavior
			{
			public:
				void ExecuteAI() override
				{
					std::cout << "몬스터가 공격적으로 행동합니다!" << std::endl;
				}
			};

			// 방어적인 AI
			class DefensiveAI : public IAIBehavior
			{
			public:
				void ExecuteAI() override
				{
					std::cout << "몬스터가 방어적으로 행동합니다!" << std::endl;
				}
			};

			// 몬스터 기본 클래스 (AI를 브리지 패턴으로 포함)
			class Monster
			{
			protected:
				std::shared_ptr<IAIBehavior> aiBehavior; // AI를 멤버로 포함

			public:
				Monster(std::shared_ptr<IAIBehavior> ai) : aiBehavior(ai) {}

				virtual void Attack() = 0;

				void PerformAI()
				{
					aiBehavior->ExecuteAI();
				}

				virtual ~Monster() = default;
			};

			// 오크 몬스터
			class Orc : public Monster
			{
			public:
				Orc(std::shared_ptr<IAIBehavior> ai) : Monster(ai) {}

				void Attack() override
				{
					std::cout << "오크가 도끼로 공격!" << std::endl;
				}
			};

			// 드래곤 몬스터
			class SkeletonKnight : public Monster
			{
			public:
				SkeletonKnight(std::shared_ptr<IAIBehavior> ai) : Monster(ai) {}

				void Attack() override
				{
					std::cout << "해골 기사가 칼로 공격!" << std::endl;
				}
			};

			// 테스트 함수
			static void Test()
			{
				// 몬스터 클래스의 변화와 AI 부분의 변화가 독립적으로 이루어지는 것을 확인할 수 있다.
				std::shared_ptr<IAIBehavior> aggressiveAI = std::make_shared<AggressiveAI>();
				std::shared_ptr<IAIBehavior> defensiveAI = std::make_shared<DefensiveAI>();

				std::shared_ptr<Monster> orc = std::make_shared<Orc>(aggressiveAI);
				std::shared_ptr<Monster> skeletonKnight = std::make_shared<SkeletonKnight>(defensiveAI);

				orc->Attack();
				orc->PerformAI(); // AI 실행

				skeletonKnight->Attack();
				skeletonKnight->PerformAI(); // AI 실행
			}
		}

		namespace Composite
		{
			// 트리 구조의 전체 - 부분 구조가 되도록 클래스 구조를 잡는 것
			// 클라이언트 입장에서 통일된 방식으로 구성원을 관리하기 위해 사용.
			// 구조 중첩 패턴을 통해 부모 자식 관계를 만들 수 있음.이 때에 리프 노드에 대한 처리가 필요.
			// 책임 연쇄 패턴을 활용 가능

			// 컴포넌트 인터페이스
			class Component
			{
			public:
				virtual ~Component() = default;
				virtual void Update() = 0;
				virtual bool HandleEvent(const std::string& event)
				{
					return false; // 기본적으로 이벤트를 처리하지 않음
				}
			};

			// 위치(Transform) 컴포넌트
			class TransformComponent : public Component
			{
			public:
				void Update() override
				{
					std::cout << "TransformComponent 업데이트\n";
				}

				bool HandleEvent(const std::string& event) override
				{
					if (event == "MOVE")
					{
						std::cout << "TransformComponent: 이동 이벤트 처리\n";
						return true;
					}
					return false;
				}
			};

			// 렌더(Render) 컴포넌트
			class RenderComponent : public Component
			{
			public:
				void Update() override
				{
					std::cout << "RenderComponent 업데이트\n";
				}

				bool HandleEvent(const std::string& event) override
				{
					if (event == "RENDER")
					{
						std::cout << "RenderComponent: 렌더링 이벤트 처리\n";
						return true;
					}
					return false;
				}
			};

			// 게임 오브젝트 (Composite)
			class GameObject
			{
			public:
				explicit GameObject(const std::string& name) : name(name) {}

				void AddComponent(std::shared_ptr<Component> component)
				{
					components.push_back(component);
				}

				void AddChild(std::shared_ptr<GameObject> child)
				{
					children.push_back(child);
				}

				void Update()
				{
					std::cout << "GameObject [" << name << "] 업데이트 시작\n";

					for (auto& component : components)
					{
						component->Update();
					}

					for (auto& child : children)
					{
						child->Update();
					}

					std::cout << "GameObject [" << name << "] 업데이트 완료\n";
				}

				// 부모가 처리하지 못한 이벤트를 자식들에게 전파
				void HandleEvent(const std::string& event)
				{
					std::cout << "GameObject [" << name << "] 이벤트 처리 시도: " << event << "\n";

					// 먼저 자신이 가진 컴포넌트들이 이벤트 처리 시도
					for (auto& component : components)
					{
						if (component->HandleEvent(event))
						{
							std::cout << "GameObject [" << name << "]에서 이벤트 [" << event << "] 처리됨\n";
							return;
						}
					}

					// 자신이 처리하지 못했으면 자식들에게 이벤트 전달
					for (auto& child : children)
					{
						child->HandleEvent(event);
					}
				}

			private:
				std::string name;
				std::vector<std::shared_ptr<Component>> components;
				std::vector<std::shared_ptr<GameObject>> children;
			};

			void Test()
			{
				auto root = std::make_shared<GameObject>("Root");

				auto player = std::make_shared<GameObject>("Player");
				auto enemy = std::make_shared<GameObject>("Enemy");

				player->AddComponent(std::make_shared<TransformComponent>());
				player->AddComponent(std::make_shared<RenderComponent>());

				enemy->AddComponent(std::make_shared<TransformComponent>());

				root->AddChild(player);
				root->AddChild(enemy);

				root->Update();

				// 이벤트 처리 테스트
				std::cout << "\n이벤트 처리 테스트 시작\n";
				root->HandleEvent("MOVE");
				root->HandleEvent("RENDER");
				root->HandleEvent("ATTACK"); // 처리할 컴포넌트 없음
			}
		}

		namespace Decorator
		{
			/*
			동적으로 객체에 대한 책임을 추가하는 것
			상속대신 기능을 추가할 수 있는 방법.
			기존에 내가 고치기 힘든 클래스를 사용하고 여기에 기능을 추가할 때에 사용할 수 있다.
			재귀적으로 구성을 쓰는 구조가 쓰인다. (계속 래핑해서 책임을 추가하기 때문에)
			컴포넌트가 무거우면 데코레이터말고 전략패턴 쓰는 것이 더 낫다.
			주로 그래픽 처리할 때에 사용하거나 스트림 입출력 시에 선후처리를 하기 위해서 사용한다.
			구성이랑 비슷해보이지만 사용 의도가 다르다.
			*/

			// 캐릭터 상태 인터페이스
			class ICharacterState
			{
			public:
				virtual ~ICharacterState() = default;
				virtual void Apply(class Character* character) = 0;
			};

			// 캐릭터 클래스
			class Character
			{
			private:
				int speed;
				bool canUseSkill;

				int baseSpeed; // 최초 속도 값 유지
				bool baseCanUseSkill; // 최초 스킬 사용 가능 여부 유지

				std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> states;

			public:
				Character() : speed(10), canUseSkill(true), baseSpeed(10), baseCanUseSkill(true) {}

				void Move()
				{
					std::cout << "이동 속도: " << speed << std::endl;
				}

				void UseSkill()
				{
					if (canUseSkill)
					{
						std::cout << "스킬 사용 가능" << std::endl;
					}
					else
					{
						std::cout << "스킬 사용 불가" << std::endl;
					}
				}

				void SetSpeed(int s) { speed = s; }
				int GetSpeed() { return speed; }

				void SetCanUseSkill(bool flag) { canUseSkill = flag; }
				bool GetCanUseSkill() { return canUseSkill; }

				// 상태 추가
				template <typename T, typename... Args>
				void AddState(Args &&... args)
				{
					std::size_t key = typeid(T).hash_code();

					if (states.find(key) != states.end())
					{
						return; // 중복 상태 추가 방지
					}

					if (states.empty())
					{
						baseSpeed = speed;
						baseCanUseSkill = canUseSkill;
					}

					auto state = std::make_shared<T>(std::forward<Args>(args)...);
					states[key] = state;
					ApplyStates(); // 새 상태 적용
				}

				// 상태 제거
				template <typename T>
				void RemoveState()
				{
					std::size_t key = typeid(T).hash_code();

					if (states.find(key) != states.end())
					{
						states.erase(key);
						ApplyStates(); // 상태 제거 후 다시 상태 적용
					}
				}

				// 모든 상태 재적용
				void ApplyStates()
				{
					speed = baseSpeed;
					canUseSkill = baseCanUseSkill;

					for (auto& pair : states)
					{
						pair.second->Apply(this);
					}
				}
			};

			// 출혈 상태 (이동 속도 감소)
			class BleedingState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(character->GetSpeed() * 0.7);
					std::cout << "출혈 상태 적용: 이동 속도 감소 (" << character->GetSpeed() << ")" << std::endl;
				}
			};

			// 침묵 상태 (스킬 사용 불가)
			class SilenceState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetCanUseSkill(false);
					std::cout << "침묵 상태 적용: 스킬 사용 불가" << std::endl;
				}
			};

			// 기절 상태 (이동 및 스킬 사용 불가)
			class StunState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(0);
					character->SetCanUseSkill(false);
					std::cout << "기절 상태 적용: 이동 불가, 스킬 사용 불가" << std::endl;
				}
			};

			// 순서 무관 테스트 함수
			void Test()
			{
				std::shared_ptr<Character> myCharacter = std::make_shared<Character>();

				std::cout << "기본 캐릭터 상태:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 침묵 상태 먼저 추가
				myCharacter->AddState<SilenceState>();
				std::cout << "침묵 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 출혈 상태 추가
				myCharacter->AddState<BleedingState>();
				std::cout << "침묵 + 출혈 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 기절 상태 추가
				myCharacter->AddState<StunState>();
				std::cout << "침묵 + 출혈 + 기절 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 침묵 상태 제거 (출혈 상태 유지)
				std::cout << "침묵 상태 제거 후:" << std::endl;
				myCharacter->RemoveState<SilenceState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 기절 상태 제거 (출혈 상태 유지)
				std::cout << "기절 상태 제거 후:" << std::endl;
				myCharacter->RemoveState<StunState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 출혈 상태 제거 (모든 상태 제거됨)
				std::cout << "출혈 상태 제거 후 (모든 상태 해제):" << std::endl;
				myCharacter->RemoveState<BleedingState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;
			}
		}

		namespace Facade
		{
			// 여러 인터페이스를 합쳐서 통합된 형태의 인터페이스를 제공하는 것.
			// 여러 인터페이스들은 보통 서브시스템의 인터페이스인데, 이렇게 서브시스템 인터페이스를 클라이언트에서 숨겨서 서브시스템 인터페이스 변화에 대한 독립성을 보장하는 것이 목표이다.
			// 보통 퍼사드 객체는 싱글톤이다.
			// 추상 팩토리와 같이 쓰일 수 있다.

			// 서브 시스템 인터페이스 정의
			class IRenderer
			{
			public:
				virtual void Render() = 0;
				virtual ~IRenderer() = default;
			};

			class IPhysics
			{
			public:
				virtual void UpdatePhysics() = 0;
				virtual ~IPhysics() = default;
			};

			class IAudio
			{
			public:
				virtual void PlaySound() = 0;
				virtual ~IAudio() = default;
			};

			// 서브 시스템의 기본 구현 (Basic)
			class BasicRenderer : public IRenderer
			{
			public:
				void Render() override
				{
					std::cout << "BasicRenderer: Simple rendering.\n";
				}
			};

			class BasicPhysics : public IPhysics
			{
			public:
				void UpdatePhysics() override
				{
					std::cout << "BasicPhysics: Basic physics calculation.\n";
				}
			};

			class BasicAudio : public IAudio
			{
			public:
				void PlaySound() override
				{
					std::cout << "BasicAudio: Playing basic sound.\n";
				}
			};

			// 서브 시스템의 고급 구현 (Advanced)
			class AdvancedRenderer : public IRenderer
			{
			public:
				void Render() override
				{
					std::cout << "AdvancedRenderer: High-quality rendering with effects.\n";
				}
			};

			class RealisticPhysics : public IPhysics
			{
			public:
				void UpdatePhysics() override
				{
					std::cout << "RealisticPhysics: Realistic physics simulation.\n";
				}
			};

			class SurroundAudio : public IAudio
			{
			public:
				void PlaySound() override
				{
					std::cout << "SurroundAudio: Playing surround sound.\n";
				}
			};

			// 추상 팩토리 패턴 적용
			class GameSystemFactory
			{
			public:
				virtual std::shared_ptr<IRenderer> CreateRenderer() = 0;
				virtual std::shared_ptr<IPhysics> CreatePhysics() = 0;
				virtual std::shared_ptr<IAudio> CreateAudio() = 0;
				virtual ~GameSystemFactory() = default;
			};

			// 기본적인 서브 시스템을 생성하는 팩토리
			class BasicGameSystemFactory : public GameSystemFactory
			{
			public:
				std::shared_ptr<IRenderer> CreateRenderer() override
				{
					return std::make_shared<BasicRenderer>();
				}

				std::shared_ptr<IPhysics> CreatePhysics() override
				{
					return std::make_shared<BasicPhysics>();
				}

				std::shared_ptr<IAudio> CreateAudio() override
				{
					return std::make_shared<BasicAudio>();
				}
			};

			// 고급 서브 시스템을 생성하는 팩토리
			class AdvancedGameSystemFactory : public GameSystemFactory
			{
			public:
				std::shared_ptr<IRenderer> CreateRenderer() override
				{
					return std::make_shared<AdvancedRenderer>();
				}

				std::shared_ptr<IPhysics> CreatePhysics() override
				{
					return std::make_shared<RealisticPhysics>();
				}

				std::shared_ptr<IAudio> CreateAudio() override
				{
					return std::make_shared<SurroundAudio>();
				}
			};

			// Facade 클래스 (싱글톤, 두 개의 버전)
			class GameFacade
			{
			protected:
				std::shared_ptr<IRenderer> _renderer;
				std::shared_ptr<IPhysics> _physics;
				std::shared_ptr<IAudio> _audio;

				GameFacade(std::shared_ptr<GameSystemFactory> factory)
				{
					_renderer = factory->CreateRenderer();
					_physics = factory->CreatePhysics();
					_audio = factory->CreateAudio();
				}

			public:
				virtual void RunGameLoop()
				{
					std::cout << "GameFacade: Running game loop.\n";
					_renderer->Render();
					_physics->UpdatePhysics();
					_audio->PlaySound();
				}

				virtual ~GameFacade() = default;
			};

			// 기본 버전의 게임 퍼사드
			class GameFacadeA : public GameFacade
			{
			private:
				GameFacadeA() : GameFacade(std::make_shared<BasicGameSystemFactory>()) {}

			public:
				static GameFacadeA& GetInstance()
				{
					static GameFacadeA instance;
					return instance;
				}
			};

			// 고급 버전의 게임 퍼사드
			class GameFacadeB : public GameFacade
			{
			private:
				GameFacadeB() : GameFacade(std::make_shared<AdvancedGameSystemFactory>()) {}

			public:
				static GameFacadeB& GetInstance()
				{
					static GameFacadeB instance;
					return instance;
				}
			};

			// 테스트 코드
			void Test()
			{
				std::cout << "[Test: Basic Game System]\n";
				GameFacadeA& gameA = GameFacadeA::GetInstance();
				gameA.RunGameLoop();

				std::cout << "\n[Test: Advanced Game System]\n";
				GameFacadeB& gameB = GameFacadeB::GetInstance();
				gameB.RunGameLoop();
			}
		
		}

		namespace FlyWeight
		{
			// 객체 일부 공유를 통해 다른 여러 객체를 효율적으로 지원하는 패턴(공유하는 부분과 안하고 독립적인 부분을 구별하는 구조를 제공함)
			// 특정 반복되는 데이터가 많은 객체에서 사용되고, 이 객체들의 참조를 써서 공유해도 로직상 문제가 없는 경우에 사용.
			// (객체가 같음을 판별할 때에 같은 참조를 쓰면 안되는 경우에는 사용하기 어렵단 얘기)
			// 전략, 상태 객체를 플라이웨이트로 쓰면 좋다.
			// 보통 Composite패턴과 함께 쓰인다.
		}

		namespace Proxy
		{

		}
	}


	// 클래스 간의 기능 분배 및 통신 방법에 대해 집중적으로 논하는 패턴
	namespace Behavioural
	{
		namespace ChainOfResponsibility
		{

		}

		namespace Command
		{

		}

		namespace Interpreter
		{

		}

		namespace Iterator
		{

		}

		namespace Mediator
		{

		}

		namespace Memento
		{

		}

		namespace Observer
		{

		}

		namespace State
		{

		}

		namespace Strategy
		{

		}

		namespace TemplateMethod
		{

		}

		namespace Visitor
		{

		}
	}
}