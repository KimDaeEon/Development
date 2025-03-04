#pragma once
namespace GOF
{
	namespace Granularity
	{
		// ����ȭ ������ Ŀ�� ��ɺ��� �Լ��� �������� �ִ�.
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

		// ����ȭ ������ �۾Ƽ� ����� ū ����� �۵��Ѵ�. �Լ� ȣ���� �� ��������.
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


	// ��ü ���� ������ �߻�ȭ��Ų ���ϵ�
	namespace Creational
	{
		namespace AbstractFactory
		{
			// ������� Ŭ���� ������ ���丮 �߻� Ŭ������ ����ü�μ� �����ϴ� ��.
			// �� ���� ���丮�� �����ϴ� ��ü�� �߻�ȭ�� Ŭ������ �����Ѵ�.

			// 1. ��ƼƼ �� ���� �������̽�
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

			// 2. ���� & �÷��̾� (��ƼƼ)
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

			// 3. ���� ����
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

			// 4. �߻� ���丮 �������̽� (���丮 �޼��� ����)
			class GameFactory
			{
			public:
				// ��ó�� ���� ���� FactoryMethod �� �����ؼ� �ϳ��� ��ɰ� ���õ� ���� Ŭ�������� ������ å������.
				virtual std::shared_ptr<Entity> CreateCharacter() = 0;
				virtual std::shared_ptr<Weapon> CreateWeapon() = 0;
				virtual ~GameFactory() = default;
			};

			// 5. ��Ÿ�� ���� ���丮 (�߻� ���丮 ����)
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

			// 6. ���� ���� ���丮 (�߻� ���丮 ����)
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

			// 7. �׽�Ʈ �ڵ�
			void Test()
			{
				std::shared_ptr<GameFactory> factory;

				// ��Ÿ�� ���� ���丮
				factory = std::make_shared<CharacterGameFactory>();
				std::shared_ptr<Entity> player = factory->CreateCharacter();
				std::shared_ptr<Weapon> sword = factory->CreateWeapon();
				player->Display();  // "Player Created!"
				sword->Use();       // "Swinging a sword!"

				// ���� ���� ���丮
				factory = std::make_shared<MonsterGameFactory>();
				std::shared_ptr<Entity> monster = factory->CreateCharacter();
				std::shared_ptr<Weapon> bow = factory->CreateWeapon();
				monster->Display();  // "Monster Created!"
				bow->Use();          // "Shooting an arrow!"
			}
		}

		namespace FactoryMethod
		{
			// ��ü ������ ���� Ŭ�������� ����ϵ��� �Ͽ�, ��ü ���� �ڵ�� ��ü ��� �ڵ带 �и��ϴ� ����.
			// �⺻������ �������̽��� �߻� Ŭ������ �����ϰ�, ���� ��ü ������ �̸� ����� ���� Ŭ�������� ������.
			// �� ������ ����ϸ� Ŭ���̾�Ʈ �ڵ尡 ��ü���� Ŭ������ �ν��Ͻ��� ���� �������� �ʰ�, �������̽��� ���� ��ü�� ������ �� �ֵ��� ������.
			// �� ����� ���Ͽ� ���� �ʾƵ� Ư�� �޼��尡 �ٸ� Ŭ������ ��ü ������ �޼��� ���ο��� ĸ��ȭ �Ѵٸ� �а� ���� ���� �� �������� �� �� �ִٰ� �Ѵ�.

			namespace UsingInheritance
			{
				class Entity
				{
				public:
					virtual ~Entity() = default;
					virtual void Attack() const = 0;

					// ���丮 �޼��� (����� ���� ��ü ���� å�� ����)
					virtual std::unique_ptr<Entity> Create() const = 0;
				};

				// ���� Ŭ����
				class Monster : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "���Ͱ� �����մϴ�!\n";
					}

					std::unique_ptr<Entity> Create() const override
					{
						return std::make_unique<Monster>();
					}
				};

				// �÷��̾� Ŭ����
				class Player : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "�÷��̾ �����մϴ�!\n";
					}

					std::unique_ptr<Entity> Create() const override
					{
						return std::make_unique<Player>();
					}
				};

				// MyPlayer Ŭ���� (Player ���)
				class MyPlayer : public Player
				{
				public:
					void Attack() const override
					{
						std::cout << "������ ĳ���Ͱ� �����մϴ�!\n";
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
				// �⺻ ��ƼƼ Ŭ����
				class Entity
				{
				public:
					virtual ~Entity() = default;
					virtual void Attack() const = 0;

					// ���丮 �޼��� (���ڿ��� �̿��� ��ü ����)
					static std::unique_ptr<Entity> Create(const std::string& type);
				};

				// ���� Ŭ����
				class Monster : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "���Ͱ� �����մϴ�!\n";
					}
				};

				// �÷��̾� Ŭ����
				class Player : public Entity
				{
				public:
					void Attack() const override
					{
						std::cout << "�÷��̾ �����մϴ�!\n";
					}
				};

				// MyPlayer Ŭ���� (Player ���)
				class MyPlayer : public Player
				{
				public:
					void Attack() const override
					{
						std::cout << "������ ĳ���Ͱ� �����մϴ�!\n";
					}
				};

				// ���丮 �޼��� ����
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
					return nullptr; // �߸��� Ÿ�� ó��
				}

				// ���� �ڵ�
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
			// Clone �̶�� �������̽��� �����ϵ��� �����Ѵ�. �̸� ���� ��� ���迡�� �������� Ȱ���Ͽ� �������� �ùٸ��� ������ �� �ֵ��� �Ѵ�.
			// �̹� �����ϴ� ��ü�� �����Ͽ� ���ο� ��ü�� �����ϰ� ���� ��, ������ �Ӽ��� ���� ��ü�� ������ ����� ���� ���� ����� �� �ִ�.
			// ���丮 �޼��峪 �߻� ���丮 ���Ͽ� ������ ���� ���� �� ������, ������ ���ڷ����� ���Ͽ��� ������Ÿ�� ���� ���� ��찡 ����.

		// 1. Clone �������̽�
			class Entity
			{
			public:
				virtual std::shared_ptr<Entity> Clone() = 0;
				virtual void Display() = 0;
				virtual ~Entity() = default;
			};

			// 2. ������Ʈ (���� ���) - �������� Ư�� �ο� ����
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
					std::cout << "A ������Ʈ ȿ�� ����\n";
				}
			};

			class BComponent : public Component
			{
			public:
				void ApplyEffect() override
				{
					std::cout << "B ������Ʈ ȿ�� ����\n";
				}
			};

			// 3. Character Ŭ���� (������Ʈ ��� ���� ����)
			class Character : public Entity
			{
			private:
				std::vector<std::shared_ptr<Component>> components;

			public:
				Character() = default;

				// ���� ������ (������Ʈ�� ����)
				Character(const Character& other)
				{
					for (const auto& comp : other.components)
					{
						components.push_back(comp); // ���� ���� (����)
					}
				}

				std::shared_ptr<Entity> Clone() override
				{
					// ���� �����ڸ� ���ؼ� Clone ��� �۵�
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

			// 4. ���ڷ����� ���� - ĳ���Ϳ� �߰� ��� �ο�
			// ��ø������ � ����� �߰��ϰų�, ���� ��ĥ �� ���� �ڵ忡 ���� ����� ���ϰ� ���� ���� ���� ����. 
			// ��ü���� ���ô� �Ʒ��� ���ڷ����� ���Ͽ��� ����

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
					std::cout << "�߰� Ư��: " << extraFeature << "\n";
				}
			};

			// 5. ������Ÿ�� ���丮 (���丮 �޼��� ����)
			// ���丮 ���� Ŭ������ ���� �ʿ������ٸ� �߻� ���丮�� �Ʒ��� ���� prototypes ���� ���� Ŭ�������� �ٸ��� �����ϵ��� �Ͽ� ���� �����ϴ�.
			class EntityFactory
			{
			private:
				std::unordered_map<std::string, std::shared_ptr<Entity>> prototypes;

			public:
				void RegisterPrototype(const std::string& key, std::shared_ptr<Entity> prototype)
				{
					prototypes[key] = prototype;
				}

				// ���丮 �޼��� ���θ� ProtoType �Լ��� ����Ͽ� ����
				std::shared_ptr<Entity> CreateEntity(const std::string& key)
				{
					if (prototypes.find(key) != prototypes.end())
					{
						return prototypes[key]->Clone();
					}
					return nullptr;
				}
			};

			// 6. �׽�Ʈ �ڵ�
			static void Test()
			{
				EntityFactory factory;

				// �⺻ ĳ���� ��� (������Ÿ��)
				std::shared_ptr<Character> baseCharacter = std::make_shared<Character>();
				factory.RegisterPrototype("BasicCharacter", baseCharacter);

				// ĳ���� ���� �� ������Ʈ �߰� (���� ���� ����)
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

				// ���ڷ����� ���� (�߰� ��� �ο�)
				std::shared_ptr<Entity> armoredChar = std::make_shared<CharacterDecorator>(strongChar, "Armor Boost");
				armoredChar->Display();
			}
		}

		namespace Builder
		{
			// ��ü�� ���� ������ ǥ�� ���(���������� ������ ���)�� �и��Ͽ� ������ ���� �������� ���� �ٸ� ǥ���� ��ü�� ���� �� �ֵ��� �ϴ� ����.
			// ������ ��ü�� �ܰ������� ������ �� �����ϸ�, �����ڿ� ���� �Ű������� �����ϴ� ������ �ذ��ϴ� �� ������ ��.
			// ����(Director)�� ����(Builder) �������̽��� ����Ͽ� ��ü�� �����ϰ�, Ŭ���̾�Ʈ�� ������ ���� �ʿ��� ��ü�� ������ �� ����.

			// Monster Ŭ���� (������ ��ü)
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
					std::cout << "���� ����: " << name << "\n"
						<< "ü��: " << hp << "\n"
						<< "���ݷ�: " << attackPower << "\n"
						<< "Ư�� �ɷ�: " << specialAbility << "\n"
						<< "----------------------\n";
				}
			};

			// MonsterBuilder �������̽�
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

			// Concrete Builder - ���� ���� ����
			class BossMonsterBuilder : public IMonsterBuilder
			{
			private:
				std::shared_ptr<Monster> monster;

			public:
				BossMonsterBuilder() { Reset(); }

				void Reset() { monster = std::make_shared<Monster>(); }

				void BuildName() override { monster->SetName("����� ����"); }
				void BuildHP() override { monster->SetHP(5000); }
				void BuildAttackPower() override { monster->SetAttackPower(250); }
				void BuildSpecialAbility() override { monster->SetSpecialAbility("���� �ı� ��ų"); }

				std::shared_ptr<Monster> GetResult() override
				{
					auto result = monster;
					Reset(); // ���� ��ü ������ ���� ����
					return result;
				}
			};

			// Concrete Builder - �Ϲ� ���� ����
			class NormalMonsterBuilder : public IMonsterBuilder
			{
			private:
				std::shared_ptr<Monster> monster;

			public:
				NormalMonsterBuilder() { Reset(); }

				void Reset() { monster = std::make_shared<Monster>(); }

				void BuildName() override { monster->SetName("������"); }
				void BuildHP() override { monster->SetHP(100); }
				void BuildAttackPower() override { monster->SetAttackPower(10); }
				void BuildSpecialAbility() override { monster->SetSpecialAbility("���� ����"); }

				std::shared_ptr<Monster> GetResult() override
				{
					auto result = monster;
					Reset(); // ���� ��ü ������ ���� ����
					return result;
				}
			};

			// Director - ���� ���� ���� ������
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

			// Test �Լ� (���� ���� ����)
			void Test()
			{
				std::shared_ptr<BossMonsterBuilder> bossBuilder = std::make_shared<BossMonsterBuilder>();
				std::shared_ptr<NormalMonsterBuilder> normalBuilder = std::make_shared<NormalMonsterBuilder>();
				MonsterDirector director;

				std::cout << "�Ϲ� ���� ���� (�⺻ �Ӽ���):\n";
				director.SetBuilder(normalBuilder);
				director.ConstructBasicMonster();
				std::shared_ptr<Monster> normalMonster = normalBuilder->GetResult();
				normalMonster->Show();

				std::cout << "���� ���� ���� (Ǯ ����):\n";
				director.SetBuilder(bossBuilder);
				director.ConstructFullMonster();
				std::shared_ptr<Monster> bossMonster = bossBuilder->GetResult();
				bossMonster->Show();
			}
		}

		namespace Singleton
		{
			// �̱��� ���ø� Ŭ���� (��� ����)
			template <typename T>
			class Singleton
			{
			protected:
				Singleton() {}

			public:
				// ���� �� �Ҵ��� ���ϵ��� ����
				Singleton(const Singleton&) = delete;
				Singleton& operator=(const Singleton&) = delete;

				static T& GetInstance()
				{
					static T instance;
					return instance;
				}
			};

			// �ڽ� Ŭ���� 1
			class GameManager : public Singleton<GameManager>
			{
				friend class Singleton<GameManager>; // Singleton���� private ������ ���� ����

			private:
				GameManager() {}

			public:
				void Show()
				{
					std::cout << "GameManager �ν��Ͻ� �����" << std::endl;
				}
			};

			// �ڽ� Ŭ���� 2
			class AudioManager : public Singleton<AudioManager>
			{
				friend class Singleton<AudioManager>;

			private:
				AudioManager() {}

			public:
				void PlaySound()
				{
					std::cout << "AudioManager���� ���� ���" << std::endl;
				}
			};

			// �׽�Ʈ �Լ�
			void Test()
			{
				GameManager& game = GameManager::GetInstance();
				game.Show();

				AudioManager& audio = AudioManager::GetInstance();
				audio.PlaySound();

				// ������ �ν��Ͻ� Ȯ��
				GameManager& game2 = GameManager::GetInstance();
				if (&game == &game2)
				{
					std::cout << "GameManager�� �� �ϳ��� �ν��Ͻ��� ������" << std::endl;
				}
			}

		}
	}


	// ���� Ŭ�������� ��ü�� ���ļ� Ư�� ������ ����� �Ͱ� ���õ� ����
	namespace Structural
	{
		namespace Adapter
		{
			// � Ŭ������ �������̽��� ���� �ý��۰� ���� ���� ���� �̸� ���߱� ���� ����Ѵ�.
			// ���� �ý��ۿ��� ����ϴ� �������̽��� ��ӹ��� Ŭ������ ��� ����ؾ� �ϴ� Ŭ���� ��ü�� ����̳� ������ ����Ͽ� �������̽��� �����.
			// Cpp������ ���� ����� �����ؼ� �������̽��� public �����, Ÿ�� ����� private ����� ���� �̸� �����Ѵ�.
			// �ٸ� ������ ���� ����� �ȵǾ Ÿ�� ��ü�� �����ϰ� �������̽��� ��ӹ޾Ƽ� �����Ѵ�.
		
			// ���ο� ���� �ý��ۿ��� ����ϴ� ��ã�� �������̽�
			class IPathfinding
			{
			public:
				virtual std::vector<std::pair<int, int>> FindPath(int startX, int startY, int endX, int endY) = 0;
				virtual ~IPathfinding() = default;
			};

			// ���� ��ã�� �ý��� (�������̽��� �ٸ�)
			class OldPathfindingSystem
			{
			public:
				void ComputePath(int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>>& path)
				{
					std::cout << "OldPathfindingSystem: ��ã�� ���� (" << startX << "," << startY << ") -> (" << endX << "," << endY << ")\n";

					// ����: �ܼ��� ���� ��� (���� ���ӿ����� A* �˰��� �� ���)
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

			// ����� Ŭ���� (���ο� ��ã�� �������̽��� �����ϸ鼭 ���� �ý����� ���ο��� ���)
			class PathfindingAdapter : public IPathfinding
			{
			private:
				std::shared_ptr<OldPathfindingSystem> oldPathfinding;

			public:
				PathfindingAdapter(std::shared_ptr<OldPathfindingSystem> oldSystem) : oldPathfinding(oldSystem) {}

				std::vector<std::pair<int, int>> FindPath(int startX, int startY, int endX, int endY) override
				{
					std::vector<std::pair<int, int>> path;
					std::cout << "PathfindingAdapter: ��ȯ �� ȣ�� -> ";
					oldPathfinding->ComputePath(startX, startY, endX, endY, path);
					return path;
				}
			};

			// ���� ĳ���� Ŭ���� (���ο� ��ã�� �ý����� ���)
			class GameCharacter
			{
			private:
				std::shared_ptr<IPathfinding> pathfinder;

			public:
				GameCharacter(std::shared_ptr<IPathfinding> pathfindingSystem) : pathfinder(pathfindingSystem) {}

				void MoveTo(int x, int y)
				{
					std::cout << "GameCharacter: �̵� ��û -> (" << x << ", " << y << ")\n";
					std::vector<std::pair<int, int>> path = pathfinder->FindPath(0, 0, x, y);

					std::cout << "���: ";
					for (auto& p : path)
					{
						std::cout << "(" << p.first << ", " << p.second << ") ";
					}
					std::cout << "\n";
				}
			};

			// �׽�Ʈ �Լ�
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
			// �߻� Ŭ���� ���� ���� �Լ� �����, �ٸ� �������̽��� ���� �۵��ϵ��� �Ͽ� �߻� Ŭ������ ��ȭ�� �������̽��� ��ȭ�� ���������� �̷���� �� �ֵ��� ���� ����.
			// ������ �͸� ���� ����Ϳ� �����ѵ�, ����� ������ �������̽��� �����ϴ� ����� Ŭ������ ��ȭ�� ���� Ŭ������ �ƴ϶�� ������ �ٸ���.

			class IAIBehavior
			{
			public:
				virtual void ExecuteAI() = 0;
				virtual ~IAIBehavior() = default;
			};

			// �������� AI
			class AggressiveAI : public IAIBehavior
			{
			public:
				void ExecuteAI() override
				{
					std::cout << "���Ͱ� ���������� �ൿ�մϴ�!" << std::endl;
				}
			};

			// ������� AI
			class DefensiveAI : public IAIBehavior
			{
			public:
				void ExecuteAI() override
				{
					std::cout << "���Ͱ� ��������� �ൿ�մϴ�!" << std::endl;
				}
			};

			// ���� �⺻ Ŭ���� (AI�� �긮�� �������� ����)
			class Monster
			{
			protected:
				std::shared_ptr<IAIBehavior> aiBehavior; // AI�� ����� ����

			public:
				Monster(std::shared_ptr<IAIBehavior> ai) : aiBehavior(ai) {}

				virtual void Attack() = 0;

				void PerformAI()
				{
					aiBehavior->ExecuteAI();
				}

				virtual ~Monster() = default;
			};

			// ��ũ ����
			class Orc : public Monster
			{
			public:
				Orc(std::shared_ptr<IAIBehavior> ai) : Monster(ai) {}

				void Attack() override
				{
					std::cout << "��ũ�� ������ ����!" << std::endl;
				}
			};

			// �巡�� ����
			class SkeletonKnight : public Monster
			{
			public:
				SkeletonKnight(std::shared_ptr<IAIBehavior> ai) : Monster(ai) {}

				void Attack() override
				{
					std::cout << "�ذ� ��簡 Į�� ����!" << std::endl;
				}
			};

			// �׽�Ʈ �Լ�
			static void Test()
			{
				// ���� Ŭ������ ��ȭ�� AI �κ��� ��ȭ�� ���������� �̷������ ���� Ȯ���� �� �ִ�.
				std::shared_ptr<IAIBehavior> aggressiveAI = std::make_shared<AggressiveAI>();
				std::shared_ptr<IAIBehavior> defensiveAI = std::make_shared<DefensiveAI>();

				std::shared_ptr<Monster> orc = std::make_shared<Orc>(aggressiveAI);
				std::shared_ptr<Monster> skeletonKnight = std::make_shared<SkeletonKnight>(defensiveAI);

				orc->Attack();
				orc->PerformAI(); // AI ����

				skeletonKnight->Attack();
				skeletonKnight->PerformAI(); // AI ����
			}
		}

		namespace Composite
		{
			// Ʈ�� ������ ��ü - �κ� ������ �ǵ��� Ŭ���� ������ ��� ��
			// Ŭ���̾�Ʈ ���忡�� ���ϵ� ������� �������� �����ϱ� ���� ���.
			// ���� ��ø ������ ���� �θ� �ڽ� ���踦 ���� �� ����.�� ���� ���� ��忡 ���� ó���� �ʿ�.
			// å�� ���� ������ Ȱ�� ����

			// ������Ʈ �������̽�
			class Component
			{
			public:
				virtual ~Component() = default;
				virtual void Update() = 0;
				virtual bool HandleEvent(const std::string& event)
				{
					return false; // �⺻������ �̺�Ʈ�� ó������ ����
				}
			};

			// ��ġ(Transform) ������Ʈ
			class TransformComponent : public Component
			{
			public:
				void Update() override
				{
					std::cout << "TransformComponent ������Ʈ\n";
				}

				bool HandleEvent(const std::string& event) override
				{
					if (event == "MOVE")
					{
						std::cout << "TransformComponent: �̵� �̺�Ʈ ó��\n";
						return true;
					}
					return false;
				}
			};

			// ����(Render) ������Ʈ
			class RenderComponent : public Component
			{
			public:
				void Update() override
				{
					std::cout << "RenderComponent ������Ʈ\n";
				}

				bool HandleEvent(const std::string& event) override
				{
					if (event == "RENDER")
					{
						std::cout << "RenderComponent: ������ �̺�Ʈ ó��\n";
						return true;
					}
					return false;
				}
			};

			// ���� ������Ʈ (Composite)
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
					std::cout << "GameObject [" << name << "] ������Ʈ ����\n";

					for (auto& component : components)
					{
						component->Update();
					}

					for (auto& child : children)
					{
						child->Update();
					}

					std::cout << "GameObject [" << name << "] ������Ʈ �Ϸ�\n";
				}

				// �θ� ó������ ���� �̺�Ʈ�� �ڽĵ鿡�� ����
				void HandleEvent(const std::string& event)
				{
					std::cout << "GameObject [" << name << "] �̺�Ʈ ó�� �õ�: " << event << "\n";

					// ���� �ڽ��� ���� ������Ʈ���� �̺�Ʈ ó�� �õ�
					for (auto& component : components)
					{
						if (component->HandleEvent(event))
						{
							std::cout << "GameObject [" << name << "]���� �̺�Ʈ [" << event << "] ó����\n";
							return;
						}
					}

					// �ڽ��� ó������ �������� �ڽĵ鿡�� �̺�Ʈ ����
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

				// �̺�Ʈ ó�� �׽�Ʈ
				std::cout << "\n�̺�Ʈ ó�� �׽�Ʈ ����\n";
				root->HandleEvent("MOVE");
				root->HandleEvent("RENDER");
				root->HandleEvent("ATTACK"); // ó���� ������Ʈ ����
			}
		}

		namespace Decorator
		{
			/*
			�������� ��ü�� ���� å���� �߰��ϴ� ��
			��Ӵ�� ����� �߰��� �� �ִ� ���.
			������ ���� ��ġ�� ���� Ŭ������ ����ϰ� ���⿡ ����� �߰��� ���� ����� �� �ִ�.
			��������� ������ ���� ������ ���δ�. (��� �����ؼ� å���� �߰��ϱ� ������)
			������Ʈ�� ���ſ�� ���ڷ����͸��� �������� ���� ���� �� ����.
			�ַ� �׷��� ó���� ���� ����ϰų� ��Ʈ�� ����� �ÿ� ����ó���� �ϱ� ���ؼ� ����Ѵ�.
			�����̶� ����غ������� ��� �ǵ��� �ٸ���.
			*/

			// ĳ���� ���� �������̽�
			class ICharacterState
			{
			public:
				virtual ~ICharacterState() = default;
				virtual void Apply(class Character* character) = 0;
			};

			// ĳ���� Ŭ����
			class Character
			{
			private:
				int speed;
				bool canUseSkill;

				int baseSpeed; // ���� �ӵ� �� ����
				bool baseCanUseSkill; // ���� ��ų ��� ���� ���� ����

				std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> states;

			public:
				Character() : speed(10), canUseSkill(true), baseSpeed(10), baseCanUseSkill(true) {}

				void Move()
				{
					std::cout << "�̵� �ӵ�: " << speed << std::endl;
				}

				void UseSkill()
				{
					if (canUseSkill)
					{
						std::cout << "��ų ��� ����" << std::endl;
					}
					else
					{
						std::cout << "��ų ��� �Ұ�" << std::endl;
					}
				}

				void SetSpeed(int s) { speed = s; }
				int GetSpeed() { return speed; }

				void SetCanUseSkill(bool flag) { canUseSkill = flag; }
				bool GetCanUseSkill() { return canUseSkill; }

				// ���� �߰�
				template <typename T, typename... Args>
				void AddState(Args &&... args)
				{
					std::size_t key = typeid(T).hash_code();

					if (states.find(key) != states.end())
					{
						return; // �ߺ� ���� �߰� ����
					}

					if (states.empty())
					{
						baseSpeed = speed;
						baseCanUseSkill = canUseSkill;
					}

					auto state = std::make_shared<T>(std::forward<Args>(args)...);
					states[key] = state;
					ApplyStates(); // �� ���� ����
				}

				// ���� ����
				template <typename T>
				void RemoveState()
				{
					std::size_t key = typeid(T).hash_code();

					if (states.find(key) != states.end())
					{
						states.erase(key);
						ApplyStates(); // ���� ���� �� �ٽ� ���� ����
					}
				}

				// ��� ���� ������
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

			// ���� ���� (�̵� �ӵ� ����)
			class BleedingState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(character->GetSpeed() * 0.7);
					std::cout << "���� ���� ����: �̵� �ӵ� ���� (" << character->GetSpeed() << ")" << std::endl;
				}
			};

			// ħ�� ���� (��ų ��� �Ұ�)
			class SilenceState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetCanUseSkill(false);
					std::cout << "ħ�� ���� ����: ��ų ��� �Ұ�" << std::endl;
				}
			};

			// ���� ���� (�̵� �� ��ų ��� �Ұ�)
			class StunState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(0);
					character->SetCanUseSkill(false);
					std::cout << "���� ���� ����: �̵� �Ұ�, ��ų ��� �Ұ�" << std::endl;
				}
			};

			// ���� ���� �׽�Ʈ �Լ�
			void Test()
			{
				std::shared_ptr<Character> myCharacter = std::make_shared<Character>();

				std::cout << "�⺻ ĳ���� ����:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ħ�� ���� ���� �߰�
				myCharacter->AddState<SilenceState>();
				std::cout << "ħ�� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� �߰�
				myCharacter->AddState<BleedingState>();
				std::cout << "ħ�� + ���� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� �߰�
				myCharacter->AddState<StunState>();
				std::cout << "ħ�� + ���� + ���� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ħ�� ���� ���� (���� ���� ����)
				std::cout << "ħ�� ���� ���� ��:" << std::endl;
				myCharacter->RemoveState<SilenceState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� ���� (���� ���� ����)
				std::cout << "���� ���� ���� ��:" << std::endl;
				myCharacter->RemoveState<StunState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� ���� (��� ���� ���ŵ�)
				std::cout << "���� ���� ���� �� (��� ���� ����):" << std::endl;
				myCharacter->RemoveState<BleedingState>();
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;
			}
		}

		namespace Facade
		{
			// ���� �������̽��� ���ļ� ���յ� ������ �������̽��� �����ϴ� ��.
			// ���� �������̽����� ���� ����ý����� �������̽��ε�, �̷��� ����ý��� �������̽��� Ŭ���̾�Ʈ���� ���ܼ� ����ý��� �������̽� ��ȭ�� ���� �������� �����ϴ� ���� ��ǥ�̴�.
			// ���� �ۻ�� ��ü�� �̱����̴�.
			// �߻� ���丮�� ���� ���� �� �ִ�.

			// ���� �ý��� �������̽� ����
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

			// ���� �ý����� �⺻ ���� (Basic)
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

			// ���� �ý����� ��� ���� (Advanced)
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

			// �߻� ���丮 ���� ����
			class GameSystemFactory
			{
			public:
				virtual std::shared_ptr<IRenderer> CreateRenderer() = 0;
				virtual std::shared_ptr<IPhysics> CreatePhysics() = 0;
				virtual std::shared_ptr<IAudio> CreateAudio() = 0;
				virtual ~GameSystemFactory() = default;
			};

			// �⺻���� ���� �ý����� �����ϴ� ���丮
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

			// ��� ���� �ý����� �����ϴ� ���丮
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

			// Facade Ŭ���� (�̱���, �� ���� ����)
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

			// �⺻ ������ ���� �ۻ��
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

			// ��� ������ ���� �ۻ��
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

			// �׽�Ʈ �ڵ�
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
			// ��ü �Ϻ� ������ ���� �ٸ� ���� ��ü�� ȿ�������� �����ϴ� ����(�����ϴ� �κа� ���ϰ� �������� �κ��� �����ϴ� ������ ������)
			// Ư�� �ݺ��Ǵ� �����Ͱ� ���� ��ü���� ���ǰ�, �� ��ü���� ������ �Ἥ �����ص� ������ ������ ���� ��쿡 ���.
			// (��ü�� ������ �Ǻ��� ���� ���� ������ ���� �ȵǴ� ��쿡�� ����ϱ� ��ƴ� ���)
			// ����, ���� ��ü�� �ö��̿���Ʈ�� ���� ����.
			// ���� Composite���ϰ� �Բ� ���δ�.
		}

		namespace Proxy
		{

		}
	}


	// Ŭ���� ���� ��� �й� �� ��� ����� ���� ���������� ���ϴ� ����
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