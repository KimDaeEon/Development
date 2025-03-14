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
				double _speed;
				bool _canUseSkill;

				double _baseSpeed; // 최초 속도 값 유지
				bool _baseCanUseSkill; // 최초 스킬 사용 가능 여부 유지

				std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> _states;

			public:
				Character() : _speed(10), _canUseSkill(true), _baseSpeed(10), _baseCanUseSkill(true) {}

				void Move()
				{
					std::cout << "이동 속도: " << _speed << std::endl;
				}

				void UseSkill()
				{
					if (_canUseSkill)
					{
						std::cout << "스킬 사용 가능" << std::endl;
					}
					else
					{
						std::cout << "스킬 사용 불가" << std::endl;
					}
				}

				void SetSpeed(double s) { _speed = s; }
				double GetSpeed() { return _speed; }

				void SetCanUseSkill(bool flag) { _canUseSkill = flag; }
				bool GetCanUseSkill() { return _canUseSkill; }

				// 상태 추가
				template <typename T, typename... Args>
				void AddState(Args &&... args)
				{
					std::size_t key = typeid(T).hash_code();

					if (_states.find(key) != _states.end())
					{
						return; // 중복 상태 추가 방지
					}

					if (_states.empty())
					{
						_baseSpeed = _speed;
						_baseCanUseSkill = _canUseSkill;
					}

					auto state = std::make_shared<T>(std::forward<Args>(args)...);
					_states[key] = state;
					ApplyStates(); // 새 상태 적용
				}

				// 상태 제거
				template <typename T>
				void RemoveState()
				{
					std::size_t key = typeid(T).hash_code();

					if (_states.find(key) != _states.end())
					{
						_states.erase(key);
						ApplyStates(); // 상태 제거 후 다시 상태 적용
					}
				}

				// 모든 상태 재적용
				void ApplyStates()
				{
					_speed = _baseSpeed;
					_canUseSkill = _baseCanUseSkill;

					for (auto& pair : _states)
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

			// -------------------------------
			// 1. 상태 인터페이스 (Flyweight 적용)
			// -------------------------------
			class Character;

			class ICharacterState
			{
			public:
				virtual ~ICharacterState() = default;
				virtual void Apply(Character* character) = 0;
			};

			// -------------------------------
			// 2. 캐릭터 클래스 (플라이웨이트 적용)
			// -------------------------------
			class Character
			{
			private:
				double speed;
				bool canUseSkill;

				double baseSpeed;
				bool baseCanUseSkill;

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

				void SetSpeed(double s) { speed = s; }
				double GetSpeed() { return speed; }

				void SetCanUseSkill(bool flag) { canUseSkill = flag; }
				bool GetCanUseSkill() { return canUseSkill; }

				// 상태 추가
				void AddState(std::shared_ptr<ICharacterState> state)
				{
					std::size_t key = typeid(*state).hash_code();

					if (states.find(key) != states.end())
					{
						return; // 중복 상태 추가 방지
					}

					if (states.empty())
					{
						baseSpeed = speed;
						baseCanUseSkill = canUseSkill;
					}

					states[key] = state;
					ApplyStates();
				}

				// 상태 제거
				void RemoveState(std::shared_ptr<ICharacterState> state)
				{
					std::size_t key = typeid(*state).hash_code();

					if (states.find(key) != states.end())
					{
						states.erase(key);
						ApplyStates();
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

			// -------------------------------
			// 3. 상태 클래스 (Flyweight 적용)
			// -------------------------------
			class BleedingState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(character->GetSpeed() * 0.7);
					std::cout << "출혈 상태 적용: 이동 속도 감소 (" << character->GetSpeed() << ")" << std::endl;
				}
			};

			class SilenceState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetCanUseSkill(false);
					std::cout << "침묵 상태 적용: 스킬 사용 불가" << std::endl;
				}
			};

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

			// -------------------------------
			// 4. 상태 Flyweight Factory (템플릿 적용)
			// -------------------------------
			class CharacterStateFactory
			{
			private:
				static std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> statePool;

			public:
				template <typename T>
				static std::shared_ptr<ICharacterState> GetState()
				{
					std::size_t key = typeid(T).hash_code();

					if (statePool.find(key) == statePool.end())
					{
						statePool[key] = std::make_shared<T>();
					}

					return statePool[key];
				}
			};

			// 정적 멤버 변수 정의
			std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> CharacterStateFactory::statePool;


			// -------------------------------
			// 5. 테스트 코드
			// -------------------------------
			void Test()
			{
				std::shared_ptr<Character> myCharacter = std::make_shared<Character>();

				std::cout << "기본 캐릭터 상태:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 침묵 상태 추가
				myCharacter->AddState(CharacterStateFactory::GetState<SilenceState>());
				std::cout << "침묵 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 출혈 상태 추가
				myCharacter->AddState(CharacterStateFactory::GetState<BleedingState>());
				std::cout << "침묵 + 출혈 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 기절 상태 추가
				myCharacter->AddState(CharacterStateFactory::GetState<StunState>());
				std::cout << "침묵 + 출혈 + 기절 상태 적용 후:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 침묵 상태 제거 (출혈 상태 유지)
				std::cout << "침묵 상태 제거 후:" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<SilenceState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 기절 상태 제거 (출혈 상태 유지)
				std::cout << "기절 상태 제거 후:" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<StunState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// 출혈 상태 제거 (모든 상태 제거됨)
				std::cout << "출혈 상태 제거 후 (모든 상태 해제):" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<BleedingState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;
			}
		}

		namespace Proxy
		{
			// 다른 객체에 대한 접근을 대행하는 객체를 쓰는 것. 
			// 객체에 대한 접근을 대행하는 객체를 Proxy 라 하고, 이 패턴을 쓰기 위해서 실제 객체의 인터페이스에 Proxy 객체의 인터페이스를 맞춘다.
			// 이를 통해 Proxy 인터페이스로 넘어온 요청을 실제 객체로 바로 넘길 수 있게 된다.
			// 스마트 포인터도 일종의 프록시임
			// 데코레이터랑 구현 방식이 비슷한데 목적이 다르다. 데코레이터는 기능을 더하는 것이 목적이고 프록시는 접근을 제한둔다는 점이 다르다.

			// -------------------------------
			// 1. 이미지 인터페이스
			// -------------------------------
			class IImage
			{
			public:
				virtual ~IImage() = default;
				virtual void Display() = 0;
			};

			// -------------------------------
			// 2. 실제 이미지 (Real Object)
			// -------------------------------
			class RealImage : public IImage
			{
			private:
				std::string fileName;

				void LoadImage()
				{
					std::cout << "[로딩 중...] " << fileName << " 이미지 로드 중...\n";
					std::this_thread::sleep_for(std::chrono::seconds(2)); // 로딩 시간 시뮬레이션
					std::cout << "[완료] " << fileName << " 이미지 로드 완료!\n";
				}

			public:
				RealImage(const std::string& file) : fileName(file)
				{
					LoadImage(); // 생성 시 실제 로드
				}

				void Display() override
				{
					std::cout << "[표시] " << fileName << " 이미지 출력\n";
				}
			};

			// -------------------------------
			// 3. 프록시 이미지 (Proxy)
			// -------------------------------
			class ProxyImage : public IImage
			{
			private:
				std::string fileName;
				std::shared_ptr<RealImage> realImage;

			public:
				ProxyImage(const std::string& file) : fileName(file) {}

				void Display() override
				{
					if (!realImage)
					{
						realImage = std::make_shared<RealImage>(fileName); // 실제 이미지 로드
					}
					realImage->Display();
				}
			};

			// -------------------------------
			// 4. 테스트 코드 (프록시 패턴 적용)
			// -------------------------------
			void Test()
			{
				std::cout << "=== 첫 번째 이미지 요청 ===\n";
				ProxyImage image1("HighResTexture.png");

				std::cout << "\n아직 이미지를 출력하지 않았음 (실제 로딩 없음)\n";

				std::cout << "\n=== 실제 이미지 출력 요청 ===\n";
				image1.Display(); // 첫 호출 시 로딩 발생

				std::cout << "\n=== 두 번째 이미지 요청 ===\n";
				ProxyImage image2("HighResTexture.png");

				std::cout << "\n=== 같은 이미지 다시 출력 ===\n";
				image2.Display(); // 캐싱된 객체 사용 (추가 로딩 없음)
			}

		}
	}


	// 클래스 간의 기능 분배 및 통신 방법에 대해 집중적으로 논하는 패턴
	namespace Behavioural
	{
		namespace ChainOfResponsibility
		{
			// 송신자와 수신자를 디커플링 시킨다.
			// 도움 메뉴에서 도움 메세지를 띄울 때에 가장 자세한 사항을 띄워야하는데, 
			// 이 메세지가 없으면 가장 일반적인 사항의 메세지를 띄워야 하는 상황이 많아서 이럴 때에 사용한다.

			// 도움말 핸들러 기본 클래스
			class HelpHandler
			{
			protected:
				std::shared_ptr<HelpHandler> nextHandler;
				std::vector<std::string> helpMessages; // 단계별 도움말 저장

			public:
				HelpHandler(const std::vector<std::string>& messages = {}) : helpMessages(messages) {}

				void SetNext(std::shared_ptr<HelpHandler> handler)
				{
					nextHandler = handler;
				}

				virtual void ShowHelp()
				{
					if (!helpMessages.empty())
					{
						for (const auto& msg : helpMessages)
						{
							std::cout << "[도움말] " << msg << std::endl;
						}
					}

					if (nextHandler)
					{
						nextHandler->ShowHelp();
					}
					else if (helpMessages.empty())
					{
						std::cout << "[기본 도움말] 사용 가능한 도움말이 없습니다.\n";
					}
				}

				virtual ~HelpHandler() {}
			};

			// UI 요소들 (버튼, 다이얼로그, 창)
			class Button : public HelpHandler
			{
			public:
				Button(const std::vector<std::string>& messages = {}) : HelpHandler(messages) {}
			};

			class Dialog : public HelpHandler
			{
			public:
				Dialog(const std::vector<std::string>& messages = {}) : HelpHandler(messages) {}
			};

			class Window : public HelpHandler
			{
			public:
				Window(const std::vector<std::string>& messages = {}) : HelpHandler(messages) {}
			};

			void Test()
			{
				// 도움말 체인 설정
				auto window = std::make_shared<Window>(std::vector<std::string>{ "이 창은 설정 메뉴입니다." });
				auto dialog = std::make_shared<Dialog>(std::vector<std::string>{ "이 다이얼로그에서는 사용자 설정을 변경할 수 있습니다.", "고급 설정은 관리자 모드에서 가능합니다." });
				auto button = std::make_shared<Button>(std::vector<std::string>{ "이 버튼은 저장 기능을 수행합니다.", "설정 변경 후 반드시 저장해야 적용됩니다." });

				dialog->SetNext(window);  // 다이얼로그 -> 창
				button->SetNext(dialog);  // 버튼 -> 다이얼로그

				std::cout << "=== 버튼 도움말 요청 ===\n";
				button->ShowHelp();  // 버튼, 다이얼로그, 창 도움말 출력

				std::cout << "\n=== 다이얼로그 도움말 요청 ===\n";
				dialog->ShowHelp();  // 다이얼로그, 창 도움말 출력

				std::cout << "\n=== 창 도움말 요청 ===\n";
				window->ShowHelp();  // 창 도움말만 출력

				std::cout << "\n=== 도움말 없는 객체 요청 ===\n";
				auto emptyHandler = std::make_shared<HelpHandler>();
				emptyHandler->ShowHelp();  // 기본 도움말 출력
			}

		}

		namespace Command
		{
			//요청을 캡슐화하는 것, 이를 통해 동작을 취소하거나 매개변수를 쉽게 바꿀 수 있는 요청이 가능하도록 하는 것.
			//콜백함수를 객체로 대체하는 것으로 볼 수 있다.
			//Composite 을 써서 여러 개의 커맨드를 합쳐서 쓰는 경우도 있다.
			//취소 기능 만들 때에 커맨드를 실행 전에 히스토리에 깊은 복사해야 할 수도 있다.
			//커맨드에 들어가는 상태 값이 변경되지 않으면 깊은 복사하지 말고 참조만 유지해도 된다.
			//취소 기능이 없어도 되면 C++ 템플릿을 써서 커맨드 만들면 서브 클래스 안 만들어도 되어서 좋다.
			

			// 기본 커맨드 인터페이스
			class ICommand
			{
			public:
				virtual void Execute() = 0;
				virtual void Undo() {} // 취소 기능 (선택적)
				virtual ~ICommand() {}
			};

			// 특정 작업을 수행하는 커맨드 (예: 메시지 출력)
			class PrintCommand : public ICommand
			{
			private:
				std::string message;

			public:
				PrintCommand(const std::string& msg) : message(msg) {}

				void Execute() override
				{
					std::cout << "[작업 실행] " << message << std::endl;
				}

				void Undo() override
				{
					std::cout << "[작업 취소] " << message << " (실행 취소됨)" << std::endl;
				}
			};

			// 여러 개의 커맨드를 묶어 실행하는 CompositeCommand
			class CompositeCommand : public ICommand
			{
			private:
				std::vector<std::shared_ptr<ICommand>> commands;

			public:
				void Add(std::shared_ptr<ICommand> cmd)
				{
					commands.push_back(cmd);
				}

				void Execute() override
				{
					for (auto& cmd : commands)
					{
						cmd->Execute();
					}
				}

				void Undo() override
				{
					for (auto it = commands.rbegin(); it != commands.rend(); ++it)
					{
						(*it)->Undo();
					}
				}
			};

			// JobQueue: 작업을 처리하는 큐
			class JobQueue
			{
			private:
				std::queue<std::shared_ptr<ICommand>> jobQueue;
				std::stack<std::shared_ptr<ICommand>> history; // 실행된 작업 저장

			public:
				void PushJob(std::shared_ptr<ICommand> job)
				{
					jobQueue.push(job);
				}

				void ExecuteNext()
				{
					if (jobQueue.empty())
					{
						std::cout << "[JobQueue] 실행할 작업이 없습니다.\n";
						return;
					}

					auto job = jobQueue.front();
					jobQueue.pop();

					job->Execute();
					history.push(job); // 실행된 작업 저장 (취소 가능)
				}

				void UndoLast()
				{
					if (history.empty())
					{
						std::cout << "[JobQueue] 실행 취소할 작업이 없습니다.\n";
						return;
					}

					auto lastJob = history.top();
					history.pop();
					lastJob->Undo();
				}
			};

			// 템플릿 기반 커맨드 (상속 없이 간단한 작업 처리)
			template <typename Func>
			class LambdaCommand : public ICommand
			{
			private:
				Func func;

			public:
				LambdaCommand(Func f) : func(f) {}

				void Execute() override
				{
					func();
				}
			};

			void Test()
			{
				JobQueue queue;

				// 단일 작업 추가
				queue.PushJob(std::make_shared<PrintCommand>("Hello, World!"));
				queue.PushJob(std::make_shared<PrintCommand>("C++ Command Pattern"));

				// 복합 작업 추가
				auto composite = std::make_shared<CompositeCommand>();
				composite->Add(std::make_shared<PrintCommand>("Step 1"));
				composite->Add(std::make_shared<PrintCommand>("Step 2"));
				composite->Add(std::make_shared<PrintCommand>("Step 3"));
				queue.PushJob(composite);

				// 템플릿 커맨드 (Lambda 기반 작업)
				queue.PushJob(std::make_shared<LambdaCommand<std::function<void()>>>([]()
					{
						std::cout << "[Lambda 작업 실행] 커스텀 작업 수행" << std::endl;
					}));

				std::cout << "=== 작업 실행 ===\n";
				queue.ExecuteNext();
				queue.ExecuteNext();
				queue.ExecuteNext();
				queue.ExecuteNext();

				std::cout << "\n=== 실행 취소 ===\n";
				queue.UndoLast();
				queue.UndoLast();
			}


		}

		namespace Interpreter
		{
			// 한 언어가 주어지고 그 언어를 해석할 수 있는 인터프리터를 사용하는 패턴
			// 문법 확장 및 변경이 쉽다.
			// 문법 구현이 쉽다.
			// 문법이 간단하고, 속도가 안 중요할 때에 쓸 수 있다.문법이 복잡하면 Parser를 쓰는 것이 더 낫다고 하였음.
			// 실질적으로는 JSON 같은 것 쓰는게 훨씬 더 나은 것 같다.

			// 인터페이스: Expression
			class Expression
			{
			public:
				virtual ~Expression() {}
				virtual int Interpret(const std::unordered_map<std::string, int>& context) const = 0;
			};

			// 변수 표현식
			class VariableExpression : public Expression
			{
			private:
				std::string name;

			public:
				explicit VariableExpression(const std::string& name) : name(name) {}

				int Interpret(const std::unordered_map<std::string, int>& context) const override
				{
					auto it = context.find(name);
					if (it != context.end())
					{
						return it->second;
					}
					return 0; // 기본값
				}
			};

			// 숫자 표현식
			class NumberExpression : public Expression
			{
			private:
				int value;

			public:
				explicit NumberExpression(int value) : value(value) {}

				int Interpret(const std::unordered_map<std::string, int>& context) const override
				{
					return value;
				}
			};

			// 연산자 표현식 (덧셈)
			class AddExpression : public Expression
			{
			private:
				std::unique_ptr<Expression> left;
				std::unique_ptr<Expression> right;

			public:
				AddExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
					: left(std::move(left)), right(std::move(right))
				{
				}

				int Interpret(const std::unordered_map<std::string, int>& context) const override
				{
					return left->Interpret(context) + right->Interpret(context);
				}
			};

			// 연산자 표현식 (곱셈)
			class MultiplyExpression : public Expression
			{
			private:
				std::unique_ptr<Expression> left;
				std::unique_ptr<Expression> right;

			public:
				MultiplyExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
					: left(std::move(left)), right(std::move(right))
				{
				}

				int Interpret(const std::unordered_map<std::string, int>& context) const override
				{
					return left->Interpret(context) * right->Interpret(context);
				}
			};

			// 테스트 함수
			void Test()
			{
				std::unordered_map<std::string, int> context = { {"x", 6}, {"y", 10} };

				// x + y * 2 표현식 생성
				std::unique_ptr<Expression> expression = std::make_unique<AddExpression>(
					std::make_unique<VariableExpression>("x"),
					std::make_unique<MultiplyExpression>(
						std::make_unique<VariableExpression>("y"),
						std::make_unique<NumberExpression>(5)));

				// 해석 및 출력
				std::cout << "Result: " << expression->Interpret(context) << std::endl;
			}

		}

		namespace Iterator
		{
			// 여러 데이터에 접근할 수 있는 방법을 내부 구현을 숨긴 상태로 일반화시켜 제공하는 패턴
			// STL 에서 쓰는 이터레이터가 딱 이거다.

			// 이터레이터 인터페이스
			template <typename T>
			class Iterator
			{
			public:
				virtual ~Iterator() {}
				virtual bool HasNext() const = 0;
				virtual T Next() = 0;
			};

			// 컨테이너 인터페이스
			template <typename T>
			class Iterable
			{
			public:
				virtual ~Iterable() {}
				virtual Iterator<T>* CreateIterator() const = 0;
			};

			// 노드 구조체 (단순 연결 리스트 사용)
			template <typename T>
			struct Node
			{
				T data;
				Node* next;
				Node(T value) : data(value), next(nullptr) {}
			};

			// 단순 연결 리스트 기반 이터레이터
			template <typename T>
			class LinkedListIterator : public Iterator<T>
			{
			public:
				LinkedListIterator(Node<T>* start) : _current(start) {}

				bool HasNext() const override
				{
					return _current != nullptr;
				}

				T Next() override
				{
					T value = _current->data;
					_current = _current->next;
					return value;
				}

			private:
				Node<T>* _current;
			};

			// 단순 연결 리스트 컨테이너
			template <typename T>
			class LinkedList : public Iterable<T>
			{
			public:
				LinkedList() : _head(nullptr), _tail(nullptr) {}

				~LinkedList()
				{
					Node<T>* current = _head;
					while (current)
					{
						Node<T>* next = current->next;
						delete current;
						current = next;
					}
				}

				void Add(T value)
				{
					Node<T>* newNode = new Node<T>(value);
					if (_tail)
					{
						_tail->next = newNode;
					}
					else
					{
						_head = newNode;
					}
					_tail = newNode;
				}

				Iterator<T>* CreateIterator() const override
				{
					return new LinkedListIterator<T>(_head);
				}

			private:
				Node<T>* _head;
				Node<T>* _tail;
			};

			// 테스트 함수
			void Test()
			{
				LinkedList<int> list;
				list.Add(10);
				list.Add(20);
				list.Add(30);

				Iterator<int>* it = list.CreateIterator();
				std::cout << "LinkedList: ";
				while (it->HasNext())
				{
					std::cout << it->Next() << " ";
				}
				std::cout << std::endl;
				delete it;
			}
		}

		namespace Mediator
		{
			// 하나의 세트의 객체들이 어떻게 상호작용할 지를 캡슐화하는 객체를 쓰는 패턴.
			// Colleague 라는 객체가 중재자 객체를 통해 메세지를 주고 받는다.
			// 이를 통해 객체간 소통 방식이 간소화되는 장점이 있다.
			// 중재자에 모든 게 다 집중되어서 중재자 자체의 로직이 복잡해질 수 있다.
			// 여러 객체가 소통하는 방식으로 만들어야 하는데, 이 중 하나의 객체만 변경해서 재사용하기 어려운 경우 사용.
			// 게임 로직을 짜다보면 Room 내에서 이것저것 처리하는 경우가 많은데, 그것을 가상함수로 바꾸면 중재자라고 할 수 있다.


			// 중재자 인터페이스 (룸 시스템)
			class RoomMediator
			{
			public:
				virtual ~RoomMediator() {}
				virtual void Attack(class Character* attacker, const std::string& targetName, int damage) = 0;
			};

			// 캐릭터 클래스 (전투 참가자)
			class Character
			{
			protected:
				std::string _name;
				int _hp;
				RoomMediator* _room;

			public:
				Character(RoomMediator* room, const std::string& name, int hp)
					: _room(room), _name(name), _hp(hp)
				{
				}

				virtual void TakeDamage(int damage)
				{
					_hp -= damage;
					std::cout << _name << " took " << damage << " damage! (HP: " << _hp << ")" << std::endl;
					if (_hp <= 0)
					{
						std::cout << _name << " has been defeated!" << std::endl;
					}
				}

				void Attack(const std::string& targetName, int damage)
				{
					_room->Attack(this, targetName, damage);
				}

				std::string GetName() const
				{
					return _name;
				}

				bool IsAlive() const
				{
					return _hp > 0;
				}
			};

			// 전투 시스템을 관리하는 Room 클래스 (중재자 역할)
			class Room : public RoomMediator
			{
			private:
				std::vector<Character*> _participants;

			public:
				void AddCharacter(Character* character)
				{
					_participants.push_back(character);
				}

				void Attack(Character* attacker, const std::string& targetName, int damage) override
				{
					Character* target = nullptr;

					// 대상 찾기
					for (Character* character : _participants)
					{
						if (character->GetName() == targetName)
						{
							target = character;
							break;
						}
					}

					if (target == nullptr)
					{
						std::cout << attacker->GetName() << " tried to attack " << targetName << ", but it is not in the room!" << std::endl;
						return;
					}

					if (!target->IsAlive())
					{
						std::cout << attacker->GetName() << " tried to attack " << target->GetName() << ", but it is already defeated!" << std::endl;
						return;
					}

					std::cout << attacker->GetName() << " attacks " << target->GetName() << " for " << damage << " damage!" << std::endl;
					target->TakeDamage(damage);
				}
			};

			// 테스트 함수
			void Test()
			{
				Room battleRoom;

				Character player(&battleRoom, "Player", 100);
				Character monster(&battleRoom, "Orc", 80);
				Character monster2(&battleRoom, "Goblin", 60);

				battleRoom.AddCharacter(&player);
				battleRoom.AddCharacter(&monster);
				battleRoom.AddCharacter(&monster2);

				player.Attack("Orc", 30);
				monster.Attack("Player", 20);
				player.Attack("Goblin", 25);
				monster2.Attack("Player", 15);
				player.Attack("Orc", 50);
				player.Attack("Orc", 50);
				monster.Attack("Player", 25);
			}
		}

		namespace Memento
		{
			// 캡슐화를 어기지 않고 객체의 상태를 저장해서 복원할 수 있도록 해주는 것.
			// 복사할 때에 비용이 크거나 복사가 빈번하면 이 패턴을 써도 효율이 떨어진다.
			// 커맨드 패턴을 통해 구현될 수 있음

			// 플레이어 상태를 저장하는 메멘토 클래스
			class PlayerMemento
			{
			private:
				int _hp;
				int _mana;
				int _level;

			public:
				PlayerMemento(int hp, int mana, int level) : _hp(hp), _mana(mana), _level(level) {}

				int GetHP() const { return _hp; }
				int GetMana() const { return _mana; }
				int GetLevel() const { return _level; }
			};

			// 플레이어 클래스 (Originator)
			class Player
			{
			private:
				int _hp;
				int _mana;
				int _level;

			public:
				Player(int hp, int mana, int level) : _hp(hp), _mana(mana), _level(level) {}

				void ShowStatus() const
				{
					std::cout << "Player Status - HP: " << _hp << ", Mana: " << _mana << ", Level: " << _level << std::endl;
				}

				// 현재 상태 저장 (Memento 생성)
				std::shared_ptr<PlayerMemento> Save() const
				{
					return std::make_shared<PlayerMemento>(_hp, _mana, _level);
				}

				// 저장된 상태로 복원
				void Restore(const std::shared_ptr<PlayerMemento>& memento)
				{
					if (memento)
					{
						_hp = memento->GetHP();
						_mana = memento->GetMana();
						_level = memento->GetLevel();
						std::cout << "Player state restored!" << std::endl;
					}
				}

				// 전투 등으로 상태 변화
				void TakeDamage(int damage)
				{
					_hp -= damage;
					if (_hp < 0)
					{
						_hp = 0;
					}
					std::cout << "Player took " << damage << " damage! (HP: " << _hp << ")" << std::endl;
				}

				void UseMana(int amount)
				{
					_mana -= amount;
					if (_mana < 0)
					{
						_mana = 0;
					}
					std::cout << "Player used " << amount << " mana! (Mana: " << _mana << ")" << std::endl;
				}

				void LevelUp()
				{
					_level++;
					_hp += 20;
					_mana += 10;
					std::cout << "Player leveled up! (Level: " << _level << ", HP: " << _hp << ", Mana: " << _mana << ")" << std::endl;
				}
			};

			// 상태를 저장하고 관리하는 Caretaker 클래스
			class SaveManager
			{
			private:
				std::vector<std::shared_ptr<PlayerMemento>> _saveSlots;

			public:
				void SaveState(const std::shared_ptr<PlayerMemento>& memento)
				{
					_saveSlots.push_back(memento);
					std::cout << "Game state saved! (Total Saves: " << _saveSlots.size() << ")" << std::endl;
				}

				std::shared_ptr<PlayerMemento> LoadState(int index)
				{
					if (index >= 0 && index < _saveSlots.size())
					{
						std::cout << "Loading save slot " << index << "..." << std::endl;
						return _saveSlots[index];
					}
					std::cout << "Invalid save slot!" << std::endl;
					return nullptr;
				}
			};

			// 테스트 함수
			void Test()
			{
				Player player(100, 50, 1);
				SaveManager saveManager;

				player.ShowStatus();

				// 첫 번째 저장
				saveManager.SaveState(player.Save());

				// 상태 변화
				player.TakeDamage(30);
				player.UseMana(20);
				player.LevelUp();

				player.ShowStatus();

				// 두 번째 저장
				saveManager.SaveState(player.Save());

				// 추가 변화
				player.TakeDamage(50);
				player.UseMana(30);
				player.LevelUp();

				player.ShowStatus();

				// 이전 상태로 복원
				player.Restore(saveManager.LoadState(0));
				player.ShowStatus();
			}
		}

		namespace Observer
		{
			// 일대다 의존성을 통하여 특정 객체의 변화를 감지하여 의존하고 있는 객체들이 자동으로 변화하도록 하는 패턴
			// 옵저버에 관심사 설정해서 특정 이벤트만 통지하게 서브젝트 객체에 등록할 수도 있다.
			// Change Manager 라는 계층 추가해서 노티파이 방식(즉각, 모아보내기)도 설정 가능하다. 여기서 Change Manager는 중재자 패턴의 중재자이다.

			// 옵저버 인터페이스
			class Observer
			{
			public:
				virtual ~Observer() {}
				virtual void OnNotify(const std::string& eventType, int value) = 0;
			};

			// 서브젝트 인터페이스
			class Subject
			{
			protected:
				std::unordered_map<std::string, std::vector<Observer*>> _observers;

			public:
				virtual ~Subject() {}

				void AddObserver(const std::string& eventType, Observer* observer)
				{
					_observers[eventType].push_back(observer);
				}

				void RemoveObserver(const std::string& eventType, Observer* observer)
				{
					auto& list = _observers[eventType];
					list.erase(std::remove(list.begin(), list.end(), observer), list.end());
				}

				void Notify(const std::string& eventType, int value)
				{
					for (auto* observer : _observers[eventType])
					{
						observer->OnNotify(eventType, value);
					}
				}
			};

			// 중재자 역할을 하는 ChangeManager
			class ChangeManager
			{
			public:
				enum NotifyMode
				{
					IMMEDIATE, // 즉각 알림
					BATCH      // 일정 시간 후 일괄 알림
				};

			private:
				NotifyMode _mode;
				std::vector<std::pair<std::string, int>> _pendingNotifications;

			public:
				ChangeManager(NotifyMode mode) : _mode(mode) {}

				void Notify(Subject& subject, const std::string& eventType, int value)
				{
					if (_mode == IMMEDIATE)
					{
						subject.Notify(eventType, value);
					}
					else
					{
						_pendingNotifications.push_back({ eventType, value });
					}
				}

				void Flush(Subject& subject)
				{
					for (const auto& event : _pendingNotifications)
					{
						subject.Notify(event.first, event.second);
					}
					_pendingNotifications.clear();
				}

				void SetMode(NotifyMode mode)
				{
					_mode = mode;
				}
			};

			// 플레이어 클래스 (Subject)
			class Player : public Subject
			{
			private:
				int _hp;
				std::vector<std::string> _inventory;
				ChangeManager& _changeManager;

			public:
				Player(int hp, ChangeManager& changeManager) : _hp(hp), _changeManager(changeManager) {}

				void TakeDamage(int damage)
				{
					_hp -= damage;
					std::cout << "Player took " << damage << " damage! (HP: " << _hp << ")" << std::endl;
					_changeManager.Notify(*this, "HP_CHANGED", _hp);
				}

				void AddItem(const std::string& item)
				{
					_inventory.push_back(item);
					std::cout << "Player obtained: " << item << std::endl;
					_changeManager.Notify(*this, "ITEM_ADDED", _inventory.size());
				}

				void FlushNotifications()
				{
					_changeManager.Flush(*this);
				}
			};

			// UI 클래스 (Observer)
			class UI : public Observer
			{
			public:
				void OnNotify(const std::string& eventType, int value) override
				{
					if (eventType == "HP_CHANGED")
					{
						std::cout << "[UI] Health Bar Updated! New HP: " << value << std::endl;
					}
					else if (eventType == "ITEM_ADDED")
					{
						std::cout << "[UI] Inventory Updated! Total Items: " << value << std::endl;
					}
				}
			};

			// 테스트 함수
			void Test()
			{
				ChangeManager changeManager(ChangeManager::BATCH);
				Player player(100, changeManager);
				UI ui;

				// UI에 특정 이벤트만 등록
				player.AddObserver("HP_CHANGED", &ui);
				player.AddObserver("ITEM_ADDED", &ui);

				// 데미지 입기 (알림은 중재자에 의해 관리됨)
				player.TakeDamage(20);
				player.TakeDamage(10);
				player.AddItem("Sword");
				player.AddItem("Potion");

				// 중재자가 모아둔 이벤트를 한 번에 처리
				std::cout << "\nFlushing notifications...\n";
				player.FlushNotifications();

				// 즉각 알림 모드로 변경
				std::cout << "\nSwitching to IMMEDIATE mode...\n";
				changeManager.SetMode(ChangeManager::IMMEDIATE);

				player.TakeDamage(15);
				player.AddItem("Shield");
			}

		}

		namespace State
		{
			// 내부 상태 변화에 따라 객체 행동 방식을 바꾸는 패턴, 상태가 바뀔 때에 객체가 바뀐 것처럼 보이게 된다.
			// 객체의 동작이 상태에 따라 '동적으로' 바뀔 때에 쓸 수 있다.
			// Enum 같은 것을 쓰면 상태 변화에 따른 동작을 여러 코드를 수정해서 바꿔야 하는데, 이 패턴을 쓰면 현재 행동을 관장하는 상태만 교체하면 되어서 좋다.
			// 꼭 능력치 관련 상태라고 고정해서 생각하기보다는 장비를 장착하고 있는 것도 상태가 될 수 있다는 것을 염두하자.
	

			class Actor; // 전방 선언
			class Monster;

			// ================================
			// CombatState 인터페이스 (중첩 가능)
			// ================================
			class CombatState
			{
			public:
				virtual ~CombatState() {}
				virtual void ApplyEffect(Actor& actor) = 0;
				virtual void RemoveEffect(Actor& actor) = 0;
			};


			// ================================
			// 몬스터 AI 상태 (한 번에 하나만 적용)
			// ================================
			class MonsterAIState
			{
			public:
				virtual ~MonsterAIState() {}
				virtual void HandleBehavior(class Monster& monster) = 0;
			};


			class Actor
			{
			protected:
				std::string _name;
				int _hp;
				int _maxHP;
				int _attack;
				int _defense;
				std::unordered_map<std::string, std::shared_ptr<CombatState>> _combatStates;

			public:
				Actor(const std::string& name, int hp, int attack, int defense)
					: _name(name), _hp(hp), _maxHP(hp), _attack(attack), _defense(defense)
				{
				}

				virtual ~Actor() {}

				std::string GetName() const { return _name; }

				virtual void TakeDamage(int damage)
				{
					int finalDamage = damage - _defense;
					if (finalDamage < 0) { finalDamage = 0; }
					_hp -= finalDamage;
					if (_hp < 0) { _hp = 0; }

					std::cout << GetName() << " took " << finalDamage << " damage! (HP: " << _hp << ")\n";
				}

				void ApplyCombatState(const std::string& stateName, std::shared_ptr<CombatState> state)
				{
					if (_combatStates.find(stateName) == _combatStates.end())
					{
						state->ApplyEffect(*this);
						_combatStates[stateName] = state;
					}
				}

				void RemoveCombatState(const std::string& stateName)
				{
					auto it = _combatStates.find(stateName);
					if (it != _combatStates.end())
					{
						it->second->RemoveEffect(*this);
						_combatStates.erase(it);
					}
				}

				void ModifyStats(int attackDelta, int defenseDelta)
				{
					_attack += attackDelta;
					_defense += defenseDelta;
				}

				void ShowStats() const
				{
					std::cout << GetName() << " -> HP: " << _hp << "/" << _maxHP << ", Attack: " << _attack << ", Defense: " << _defense << "\n";
				}

				bool IsAlive() const { return _hp > 0; }
				int GetHP() const { return _hp; }
				int GetMaxHP() const { return _maxHP; }
				int GetAttack() const { return _attack; }
				int GetDefense() const { return _defense; }

				virtual void Attack(Actor& target) = 0;
			};


			// 버프 상태 (공격력/방어력 증가)
			class BuffedState : public CombatState
			{
			private:
				int _attackBoost;
				int _defenseBoost;

			public:
				BuffedState(int attack, int defense) : _attackBoost(attack), _defenseBoost(defense) {}

				void ApplyEffect(Actor& actor) override
				{
					actor.ModifyStats(_attackBoost, _defenseBoost);
					std::cout << actor.GetName() << " received a buff! Attack + " << _attackBoost << ", Defense + " << _defenseBoost << "\n";
				}

				void RemoveEffect(Actor& actor) override
				{
					actor.ModifyStats(-_attackBoost, -_defenseBoost);
					std::cout << actor.GetName() << "'s buff expired! Stats reverted.\n";
				}
			};

			// ================================
			// 플레이어 (Character)
			// ================================
			class Character : public Actor
			{
			public:
				Character(const std::string& name, int hp, int attack, int defense)
					: Actor(name, hp, attack, defense)
				{
				}

				void Attack(Actor& target) override
				{
					std::cout << GetName() << " attacks " << target.GetName() << "!\n";
					target.TakeDamage(_attack);
				}
			};

			// 공격적인 상태
			class AggressiveState : public MonsterAIState
			{
			public:
				void HandleBehavior(Monster& monster) override;
			};

			// 광폭화 상태 (HP 30% 이하 시 적용)
			class BerserkState : public MonsterAIState
			{
			public:
				void HandleBehavior(Monster& monster) override;
			};

			// ================================
			// 몬스터 클래스 (Monster)
			// ================================
			class Monster : public Actor
			{
			private:
				std::shared_ptr<MonsterAIState> _aiState;

			public:
				Monster(const std::string& name, int hp, int attack, int defense)
					: Actor(name, hp, attack, defense), _aiState(std::make_shared<AggressiveState>())
				{
				}

				void ChangeAIState(std::shared_ptr<MonsterAIState> newState)
				{
					_aiState = newState;
				}

				void Attack(Actor& target) override
				{
					std::cout << GetName() << " attacks " << target.GetName() << " fiercely!\n";
					target.TakeDamage(_attack);
				}

				void TakeDamage(int damage) override
				{
					Actor::TakeDamage(damage);
					_aiState->HandleBehavior(*this);
				}
			};

			// AggressiveState 동작 정의
			void AggressiveState::HandleBehavior(Monster& monster)
			{
				int hpRatio = monster.GetHP() * 100 / monster.GetMaxHP();
				if (hpRatio <= 30)
				{
					std::cout << monster.GetName() << " switches to BERSERK mode!\n";
					monster.ChangeAIState(std::make_shared<BerserkState>());
					monster.ApplyCombatState("Berserk", std::make_shared<BuffedState>(50, 50));
				}
			}

			// BerserkState 동작 정의
			void BerserkState::HandleBehavior(Monster& monster)
			{
				std::cout << monster.GetName() << " is in BERSERK mode and attacks with fury!\n";
			}

			// ================================
			// 전투 시뮬레이션 (실제 전투 진행)
			// ================================
			void Battle(Character& player, Monster& enemy)
			{
				std::cout << "\n=== Battle Start: " << player.GetName() << " vs " << enemy.GetName() << " ===\n";

				player.ApplyCombatState("Buff", std::make_shared<BuffedState>(5, 2)); // 플레이어 버프 적용
				enemy.ApplyCombatState("Debuff", std::make_shared<BuffedState>(-3, -1)); // 몬스터 디버프 적용

				while (player.IsAlive() && enemy.IsAlive())
				{
					player.Attack(enemy);
					if (!enemy.IsAlive())
					{
						std::cout << enemy.GetName() << " has been defeated!\n";
						break;
					}

					enemy.Attack(player);
					if (!player.IsAlive())
					{
						std::cout << player.GetName() << " has fallen!\n";
						break;
					}
				}

				std::cout << "=== Battle End ===\n";
			}

			// ================================
			// 테스트 코드
			// ================================
			void Test()
			{
				Character player("Hero", 100, 20, 5);
				Monster enemy("Orc", 80, 15, 3);

				Battle(player, enemy);
			}

			int main()
			{
				Test();
				return 0;
			}


		}

		namespace Strategy
		{
			// 한 알고리즘 군을 만들어서 해당 알고리즘들이 서로 바뀌면서 사용할 수 있도록 하는 것.
			// 상태패턴과의 차이는 객체 내부의 상태 변경 여부라고 한다. 사용방식은 거의 비슷하다.
			// C++ 같은 경우는 정적으로 전략이 정해져도 상관이 없다면 템플릿을 써서 이걸 구현할 수도 있고, 이로 인해 서브 클래싱을 쓰지 않아도 된다.

			class IAttackStrategy
			{
			public:
				virtual ~IAttackStrategy() = default;
				virtual void Attack() = 0;
			};

			class MeleeAttack : public IAttackStrategy
			{
			public:
				void Attack() override
				{
					std::cout << "[몬스터] 근접 공격!\n";
				}
			};

			class RangeAttack : public IAttackStrategy
			{
			public:
				void Attack() override
				{
					std::cout << "[몬스터] 원거리 공격!\n";
				}
			};

			class Monster;

			class IMonsterState
			{
			public:
				virtual ~IMonsterState() = default;
				virtual void Enter(Monster& monster) = 0;
				virtual void Update(Monster& monster) = 0;
			};

			class IdleState : public IMonsterState
			{
			public:
				void Enter(Monster& monster) override;
				void Update(Monster& monster) override;
			};

			class ChasingState : public IMonsterState
			{
			public:
				void Enter(Monster& monster) override;
				void Update(Monster& monster) override;
			};

			class AttackingState : public IMonsterState
			{
			public:
				void Enter(Monster& monster) override;
				void Update(Monster& monster) override;
			};

			// ------------------- 몬스터 클래스 (전략 + 상태 통합) -------------------

			class Monster
			{
			private:
				std::unique_ptr<IAttackStrategy> attackStrategy;
				std::unique_ptr<IMonsterState> state;

			public:
				Monster(std::unique_ptr<IAttackStrategy> strategy)
					: attackStrategy(std::move(strategy)), state(std::make_unique<IdleState>())
				{
				}

				void SetAttackStrategy(std::unique_ptr<IAttackStrategy> strategy)
				{
					attackStrategy = std::move(strategy);
				}

				void Attack()
				{
					if (attackStrategy)
					{
						attackStrategy->Attack();
					}
				}

				void ChangeState(std::unique_ptr<IMonsterState> newState)
				{
					state = std::move(newState);
					state->Enter(*this);
				}

				void Update()
				{
					if (state)
					{
						state->Update(*this);
					}
				}
			};

			// ------------------- 상태 구현 -------------------

			void IdleState::Enter(Monster& monster)
			{
				std::cout << "[몬스터] 대기 상태에 들어갔습니다.\n";
			}

			void IdleState::Update(Monster& monster)
			{
				std::cout << "[몬스터] 주변을 탐색 중...\n";
				monster.ChangeState(std::make_unique<ChasingState>());
			}

			void ChasingState::Enter(Monster& monster)
			{
				std::cout << "[몬스터] 플레이어를 쫓기 시작합니다!\n";
			}

			void ChasingState::Update(Monster& monster)
			{
				std::cout << "[몬스터] 플레이어를 추격 중...\n";
				monster.ChangeState(std::make_unique<AttackingState>());
			}

			void AttackingState::Enter(Monster& monster)
			{
				std::cout << "[몬스터] 공격 태세에 돌입!\n";
			}

			void AttackingState::Update(Monster& monster)
			{
				std::cout << "[몬스터] 플레이어를 공격합니다!\n";
				monster.ChangeState(std::make_unique<IdleState>());
			}

			// ------------------- 테스트 코드 -------------------

			void Test()
			{
				std::cout << "=== 동적 전략 변경 테스트 ===\n";
				Monster monster(std::make_unique<MeleeAttack>());
				monster.Attack();

				monster.SetAttackStrategy(std::make_unique<RangeAttack>());
				monster.Attack();

				std::cout << "\n=== 몬스터 상태 패턴 테스트 ===\n";
				monster.Update();
				monster.Update();
				monster.Update();
			}

		}

		namespace TemplateMethod
		{
			// 알고리즘을 위한 틀을 만들어두고 특정 동작에 대한 것을 하위 클래스에 위임하는 것.
			// 이를 통해 전체적인 틀은 유지하면서 일정 로직만 변경할 수 있다.
			// 한 클래스에서 특정 동작들은 고정되어 있는데, 나머지만 바꿀 때에 쓴다.

			class MonsterAI
			{
			public:
				virtual ~MonsterAI() = default;

				// 템플릿 메서드 (전체 AI 흐름 고정)
				void RunAI()
				{
					DetectEnemy();
					MoveToTarget();
					Attack();  // 하위 클래스에서 구현
					Retreat();
				}

			protected:
				void DetectEnemy()
				{
					std::cout << "[AI] 적을 탐지했습니다.\n";
				}

				void MoveToTarget()
				{
					std::cout << "[AI] 목표를 향해 이동 중...\n";
				}

				void Retreat()
				{
					std::cout << "[AI] 후퇴합니다.\n";
				}

				// 공격 방식은 하위 클래스에서 구현
				virtual void Attack() = 0;
			};

			class MeleeMonster : public MonsterAI
			{
			protected:
				void Attack() override
				{
					std::cout << "[Melee] 근접 공격!\n";
				}
			};

			class RangeMonster : public MonsterAI
			{
			protected:
				void Attack() override
				{
					std::cout << "[Range] 원거리 공격!\n";
				}
			};

			void Test()
			{
				std::cout << "=== 근접 몬스터 AI 실행 ===\n";
				std::unique_ptr<MonsterAI> melee = std::make_unique<MeleeMonster>();
				melee->RunAI();

				std::cout << "\n=== 원거리 몬스터 AI 실행 ===\n";
				std::unique_ptr<MonsterAI> range = std::make_unique<RangeMonster>();
				range->RunAI();
			}
		}

		namespace Visitor
		{
			// 한 객체의 여러 요소들에서 수행될 동작을, 해당 요소들을 변경하지 않고 추가할 수 있도록 해주는 패턴. 
			// Visitor 추가가 각 요소에서 수행될 새로운 동작 추가라고 보면 된다.
			// 객체 내의 특정 객체 집합 구조가 거의 바뀌지 않을 때에, 다양한 동작이 객체 집합 구조에 적용되어야 할 때에 사용한다.
			
			// 장점
			// Element 종류는 추가되지 않고, 해당 element에 필요한 로직들이 계속 추가되어야 할 때에 쓰면 좋다.
			// 이 패턴을 쓰면 상속관계가 아닌 클래스도 순회하며 로직을 작동시킬 수 있는 점이 좋다.
			// 순회하면서 무언가 상태를 쌓아둬야할 때에도 쓰면 좋다.
			
			// 단점
			// 객체 묶음에 포함되는 객체(element) 종류가 증가한다면, 대응되는 visitor를 계속 만들어 줘야해서 사용하기에 별로다.
			// 순회하면서 정보를 쌓으려면 element가 정보 접근 가능한 인터페이스 제공해주어야 하는데, 이로 인해 캡슐화가 약화될 수 있다.
			

			class Player;
			class Monster;
			class Shop;

			class IVisitor
			{
			public:
				virtual ~IVisitor() = default;
				virtual void Visit(Player& player) = 0;
				virtual void Visit(Monster& monster) = 0;
				virtual void Visit(Shop& shop) = 0;
			};

			// ------------------- Entity 인터페이스 -------------------

			class IEntity
			{
			public:
				virtual ~IEntity() = default;
				virtual void Accept(IVisitor& visitor) = 0;
			};

			// ------------------- 구체적인 게임 오브젝트 -------------------

			class Player : public IEntity
			{
			public:
				std::string name;
				int hp;

				Player(const std::string& n, int h) : name(n), hp(h) {}

				void Accept(IVisitor& visitor) override
				{
					visitor.Visit(*this);
				}
			};

			class Monster : public IEntity
			{
			public:
				std::string type;
				int hp;

				Monster(const std::string& t, int h) : type(t), hp(h) {}

				void Accept(IVisitor& visitor) override
				{
					visitor.Visit(*this);
				}
			};

			class Shop : public IEntity
			{
			public:
				std::string shopName;

				Shop(const std::string& name) : shopName(name) {}

				void Accept(IVisitor& visitor) override
				{
					visitor.Visit(*this);
				}
			};

			// ------------------- Visitor 구현 (렌더링) -------------------

			class RenderVisitor : public IVisitor
			{
			public:
				void Visit(Player& player) override
				{
					std::cout << "[Render] 플레이어 " << player.name << " (HP: " << player.hp << ") 화면에 표시\n";
				}

				void Visit(Monster& monster) override
				{
					std::cout << "[Render] 몬스터 " << monster.type << " (HP: " << monster.hp << ") 화면에 표시\n";
				}

				void Visit(Shop& shop) override
				{
					std::cout << "[Render] 상점 '" << shop.shopName << "' 지도에 표시\n";
				}
			};

			// ------------------- Visitor 구현 (상호작용) -------------------

			class InteractionVisitor : public IVisitor
			{
			public:
				void Visit(Player& player) override
				{
					std::cout << "[Interaction] 플레이어 " << player.name << "은(는) 상호작용할 수 없음.\n";
				}

				void Visit(Monster& monster) override
				{
					std::cout << "[Interaction] 몬스터 " << monster.type << "에게 공격 가능!\n";
				}

				void Visit(Shop& shop) override
				{
					std::cout << "[Interaction] 상점 '" << shop.shopName << "'에서 아이템 구매 가능!\n";
				}
			};

			// ------------------- 게임 맵 클래스 -------------------

			class GameMap
			{
			private:
				std::vector<std::unique_ptr<IEntity>> entities;

			public:
				void AddEntity(std::unique_ptr<IEntity> entity)
				{
					entities.push_back(std::move(entity));
				}

				void ApplyVisitor(IVisitor& visitor)
				{
					for (auto& entity : entities)
					{
						entity->Accept(visitor);
					}
				}
			};

			// ------------------- 테스트 코드 -------------------

			void Test()
			{
				GameMap gameMap;

				gameMap.AddEntity(std::make_unique<Player>("Hero", 100));
				gameMap.AddEntity(std::make_unique<Monster>("Goblin", 50));
				gameMap.AddEntity(std::make_unique<Shop>("Potion Shop"));

				RenderVisitor renderer;
				InteractionVisitor interactor;

				std::cout << "=== 맵 렌더링 ===\n";
				gameMap.ApplyVisitor(renderer);

				std::cout << "\n=== 플레이어 상호작용 체크 ===\n";
				gameMap.ApplyVisitor(interactor);
			}

		}
	}
}