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
				double _speed;
				bool _canUseSkill;

				double _baseSpeed; // ���� �ӵ� �� ����
				bool _baseCanUseSkill; // ���� ��ų ��� ���� ���� ����

				std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> _states;

			public:
				Character() : _speed(10), _canUseSkill(true), _baseSpeed(10), _baseCanUseSkill(true) {}

				void Move()
				{
					std::cout << "�̵� �ӵ�: " << _speed << std::endl;
				}

				void UseSkill()
				{
					if (_canUseSkill)
					{
						std::cout << "��ų ��� ����" << std::endl;
					}
					else
					{
						std::cout << "��ų ��� �Ұ�" << std::endl;
					}
				}

				void SetSpeed(double s) { _speed = s; }
				double GetSpeed() { return _speed; }

				void SetCanUseSkill(bool flag) { _canUseSkill = flag; }
				bool GetCanUseSkill() { return _canUseSkill; }

				// ���� �߰�
				template <typename T, typename... Args>
				void AddState(Args &&... args)
				{
					std::size_t key = typeid(T).hash_code();

					if (_states.find(key) != _states.end())
					{
						return; // �ߺ� ���� �߰� ����
					}

					if (_states.empty())
					{
						_baseSpeed = _speed;
						_baseCanUseSkill = _canUseSkill;
					}

					auto state = std::make_shared<T>(std::forward<Args>(args)...);
					_states[key] = state;
					ApplyStates(); // �� ���� ����
				}

				// ���� ����
				template <typename T>
				void RemoveState()
				{
					std::size_t key = typeid(T).hash_code();

					if (_states.find(key) != _states.end())
					{
						_states.erase(key);
						ApplyStates(); // ���� ���� �� �ٽ� ���� ����
					}
				}

				// ��� ���� ������
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

			// -------------------------------
			// 1. ���� �������̽� (Flyweight ����)
			// -------------------------------
			class Character;

			class ICharacterState
			{
			public:
				virtual ~ICharacterState() = default;
				virtual void Apply(Character* character) = 0;
			};

			// -------------------------------
			// 2. ĳ���� Ŭ���� (�ö��̿���Ʈ ����)
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

				void SetSpeed(double s) { speed = s; }
				double GetSpeed() { return speed; }

				void SetCanUseSkill(bool flag) { canUseSkill = flag; }
				bool GetCanUseSkill() { return canUseSkill; }

				// ���� �߰�
				void AddState(std::shared_ptr<ICharacterState> state)
				{
					std::size_t key = typeid(*state).hash_code();

					if (states.find(key) != states.end())
					{
						return; // �ߺ� ���� �߰� ����
					}

					if (states.empty())
					{
						baseSpeed = speed;
						baseCanUseSkill = canUseSkill;
					}

					states[key] = state;
					ApplyStates();
				}

				// ���� ����
				void RemoveState(std::shared_ptr<ICharacterState> state)
				{
					std::size_t key = typeid(*state).hash_code();

					if (states.find(key) != states.end())
					{
						states.erase(key);
						ApplyStates();
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

			// -------------------------------
			// 3. ���� Ŭ���� (Flyweight ����)
			// -------------------------------
			class BleedingState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetSpeed(character->GetSpeed() * 0.7);
					std::cout << "���� ���� ����: �̵� �ӵ� ���� (" << character->GetSpeed() << ")" << std::endl;
				}
			};

			class SilenceState : public ICharacterState
			{
			public:
				void Apply(Character* character) override
				{
					character->SetCanUseSkill(false);
					std::cout << "ħ�� ���� ����: ��ų ��� �Ұ�" << std::endl;
				}
			};

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

			// -------------------------------
			// 4. ���� Flyweight Factory (���ø� ����)
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

			// ���� ��� ���� ����
			std::unordered_map<std::size_t, std::shared_ptr<ICharacterState>> CharacterStateFactory::statePool;


			// -------------------------------
			// 5. �׽�Ʈ �ڵ�
			// -------------------------------
			void Test()
			{
				std::shared_ptr<Character> myCharacter = std::make_shared<Character>();

				std::cout << "�⺻ ĳ���� ����:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ħ�� ���� �߰�
				myCharacter->AddState(CharacterStateFactory::GetState<SilenceState>());
				std::cout << "ħ�� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� �߰�
				myCharacter->AddState(CharacterStateFactory::GetState<BleedingState>());
				std::cout << "ħ�� + ���� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� �߰�
				myCharacter->AddState(CharacterStateFactory::GetState<StunState>());
				std::cout << "ħ�� + ���� + ���� ���� ���� ��:" << std::endl;
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ħ�� ���� ���� (���� ���� ����)
				std::cout << "ħ�� ���� ���� ��:" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<SilenceState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� ���� (���� ���� ����)
				std::cout << "���� ���� ���� ��:" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<StunState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;

				// ���� ���� ���� (��� ���� ���ŵ�)
				std::cout << "���� ���� ���� �� (��� ���� ����):" << std::endl;
				myCharacter->RemoveState(CharacterStateFactory::GetState<BleedingState>());
				myCharacter->Move();
				myCharacter->UseSkill();
				std::cout << "----------------------" << std::endl;
			}
		}

		namespace Proxy
		{
			// �ٸ� ��ü�� ���� ������ �����ϴ� ��ü�� ���� ��. 
			// ��ü�� ���� ������ �����ϴ� ��ü�� Proxy �� �ϰ�, �� ������ ���� ���ؼ� ���� ��ü�� �������̽��� Proxy ��ü�� �������̽��� �����.
			// �̸� ���� Proxy �������̽��� �Ѿ�� ��û�� ���� ��ü�� �ٷ� �ѱ� �� �ְ� �ȴ�.
			// ����Ʈ �����͵� ������ ���Ͻ���
			// ���ڷ����Ͷ� ���� ����� ����ѵ� ������ �ٸ���. ���ڷ����ʹ� ����� ���ϴ� ���� �����̰� ���Ͻô� ������ ���ѵдٴ� ���� �ٸ���.

			// -------------------------------
			// 1. �̹��� �������̽�
			// -------------------------------
			class IImage
			{
			public:
				virtual ~IImage() = default;
				virtual void Display() = 0;
			};

			// -------------------------------
			// 2. ���� �̹��� (Real Object)
			// -------------------------------
			class RealImage : public IImage
			{
			private:
				std::string fileName;

				void LoadImage()
				{
					std::cout << "[�ε� ��...] " << fileName << " �̹��� �ε� ��...\n";
					std::this_thread::sleep_for(std::chrono::seconds(2)); // �ε� �ð� �ùķ��̼�
					std::cout << "[�Ϸ�] " << fileName << " �̹��� �ε� �Ϸ�!\n";
				}

			public:
				RealImage(const std::string& file) : fileName(file)
				{
					LoadImage(); // ���� �� ���� �ε�
				}

				void Display() override
				{
					std::cout << "[ǥ��] " << fileName << " �̹��� ���\n";
				}
			};

			// -------------------------------
			// 3. ���Ͻ� �̹��� (Proxy)
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
						realImage = std::make_shared<RealImage>(fileName); // ���� �̹��� �ε�
					}
					realImage->Display();
				}
			};

			// -------------------------------
			// 4. �׽�Ʈ �ڵ� (���Ͻ� ���� ����)
			// -------------------------------
			void Test()
			{
				std::cout << "=== ù ��° �̹��� ��û ===\n";
				ProxyImage image1("HighResTexture.png");

				std::cout << "\n���� �̹����� ������� �ʾ��� (���� �ε� ����)\n";

				std::cout << "\n=== ���� �̹��� ��� ��û ===\n";
				image1.Display(); // ù ȣ�� �� �ε� �߻�

				std::cout << "\n=== �� ��° �̹��� ��û ===\n";
				ProxyImage image2("HighResTexture.png");

				std::cout << "\n=== ���� �̹��� �ٽ� ��� ===\n";
				image2.Display(); // ĳ�̵� ��ü ��� (�߰� �ε� ����)
			}

		}
	}


	// Ŭ���� ���� ��� �й� �� ��� ����� ���� ���������� ���ϴ� ����
	namespace Behavioural
	{
		namespace ChainOfResponsibility
		{
			// �۽��ڿ� �����ڸ� ��Ŀ�ø� ��Ų��.
			// ���� �޴����� ���� �޼����� ��� ���� ���� �ڼ��� ������ ������ϴµ�, 
			// �� �޼����� ������ ���� �Ϲ����� ������ �޼����� ����� �ϴ� ��Ȳ�� ���Ƽ� �̷� ���� ����Ѵ�.

			// ���� �ڵ鷯 �⺻ Ŭ����
			class HelpHandler
			{
			protected:
				std::shared_ptr<HelpHandler> nextHandler;
				std::vector<std::string> helpMessages; // �ܰ躰 ���� ����

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
							std::cout << "[����] " << msg << std::endl;
						}
					}

					if (nextHandler)
					{
						nextHandler->ShowHelp();
					}
					else if (helpMessages.empty())
					{
						std::cout << "[�⺻ ����] ��� ������ ������ �����ϴ�.\n";
					}
				}

				virtual ~HelpHandler() {}
			};

			// UI ��ҵ� (��ư, ���̾�α�, â)
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
				// ���� ü�� ����
				auto window = std::make_shared<Window>(std::vector<std::string>{ "�� â�� ���� �޴��Դϴ�." });
				auto dialog = std::make_shared<Dialog>(std::vector<std::string>{ "�� ���̾�α׿����� ����� ������ ������ �� �ֽ��ϴ�.", "��� ������ ������ ��忡�� �����մϴ�." });
				auto button = std::make_shared<Button>(std::vector<std::string>{ "�� ��ư�� ���� ����� �����մϴ�.", "���� ���� �� �ݵ�� �����ؾ� ����˴ϴ�." });

				dialog->SetNext(window);  // ���̾�α� -> â
				button->SetNext(dialog);  // ��ư -> ���̾�α�

				std::cout << "=== ��ư ���� ��û ===\n";
				button->ShowHelp();  // ��ư, ���̾�α�, â ���� ���

				std::cout << "\n=== ���̾�α� ���� ��û ===\n";
				dialog->ShowHelp();  // ���̾�α�, â ���� ���

				std::cout << "\n=== â ���� ��û ===\n";
				window->ShowHelp();  // â ���򸻸� ���

				std::cout << "\n=== ���� ���� ��ü ��û ===\n";
				auto emptyHandler = std::make_shared<HelpHandler>();
				emptyHandler->ShowHelp();  // �⺻ ���� ���
			}

		}

		namespace Command
		{
			//��û�� ĸ��ȭ�ϴ� ��, �̸� ���� ������ ����ϰų� �Ű������� ���� �ٲ� �� �ִ� ��û�� �����ϵ��� �ϴ� ��.
			//�ݹ��Լ��� ��ü�� ��ü�ϴ� ������ �� �� �ִ�.
			//Composite �� �Ἥ ���� ���� Ŀ�ǵ带 ���ļ� ���� ��쵵 �ִ�.
			//��� ��� ���� ���� Ŀ�ǵ带 ���� ���� �����丮�� ���� �����ؾ� �� ���� �ִ�.
			//Ŀ�ǵ忡 ���� ���� ���� ������� ������ ���� �������� ���� ������ �����ص� �ȴ�.
			//��� ����� ��� �Ǹ� C++ ���ø��� �Ἥ Ŀ�ǵ� ����� ���� Ŭ���� �� ���� �Ǿ ����.
			

			// �⺻ Ŀ�ǵ� �������̽�
			class ICommand
			{
			public:
				virtual void Execute() = 0;
				virtual void Undo() {} // ��� ��� (������)
				virtual ~ICommand() {}
			};

			// Ư�� �۾��� �����ϴ� Ŀ�ǵ� (��: �޽��� ���)
			class PrintCommand : public ICommand
			{
			private:
				std::string message;

			public:
				PrintCommand(const std::string& msg) : message(msg) {}

				void Execute() override
				{
					std::cout << "[�۾� ����] " << message << std::endl;
				}

				void Undo() override
				{
					std::cout << "[�۾� ���] " << message << " (���� ��ҵ�)" << std::endl;
				}
			};

			// ���� ���� Ŀ�ǵ带 ���� �����ϴ� CompositeCommand
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

			// JobQueue: �۾��� ó���ϴ� ť
			class JobQueue
			{
			private:
				std::queue<std::shared_ptr<ICommand>> jobQueue;
				std::stack<std::shared_ptr<ICommand>> history; // ����� �۾� ����

			public:
				void PushJob(std::shared_ptr<ICommand> job)
				{
					jobQueue.push(job);
				}

				void ExecuteNext()
				{
					if (jobQueue.empty())
					{
						std::cout << "[JobQueue] ������ �۾��� �����ϴ�.\n";
						return;
					}

					auto job = jobQueue.front();
					jobQueue.pop();

					job->Execute();
					history.push(job); // ����� �۾� ���� (��� ����)
				}

				void UndoLast()
				{
					if (history.empty())
					{
						std::cout << "[JobQueue] ���� ����� �۾��� �����ϴ�.\n";
						return;
					}

					auto lastJob = history.top();
					history.pop();
					lastJob->Undo();
				}
			};

			// ���ø� ��� Ŀ�ǵ� (��� ���� ������ �۾� ó��)
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

				// ���� �۾� �߰�
				queue.PushJob(std::make_shared<PrintCommand>("Hello, World!"));
				queue.PushJob(std::make_shared<PrintCommand>("C++ Command Pattern"));

				// ���� �۾� �߰�
				auto composite = std::make_shared<CompositeCommand>();
				composite->Add(std::make_shared<PrintCommand>("Step 1"));
				composite->Add(std::make_shared<PrintCommand>("Step 2"));
				composite->Add(std::make_shared<PrintCommand>("Step 3"));
				queue.PushJob(composite);

				// ���ø� Ŀ�ǵ� (Lambda ��� �۾�)
				queue.PushJob(std::make_shared<LambdaCommand<std::function<void()>>>([]()
					{
						std::cout << "[Lambda �۾� ����] Ŀ���� �۾� ����" << std::endl;
					}));

				std::cout << "=== �۾� ���� ===\n";
				queue.ExecuteNext();
				queue.ExecuteNext();
				queue.ExecuteNext();
				queue.ExecuteNext();

				std::cout << "\n=== ���� ��� ===\n";
				queue.UndoLast();
				queue.UndoLast();
			}


		}

		namespace Interpreter
		{
			// �� �� �־����� �� �� �ؼ��� �� �ִ� ���������͸� ����ϴ� ����
			// ���� Ȯ�� �� ������ ����.
			// ���� ������ ����.
			// ������ �����ϰ�, �ӵ��� �� �߿��� ���� �� �� �ִ�.������ �����ϸ� Parser�� ���� ���� �� ���ٰ� �Ͽ���.
			// ���������δ� JSON ���� �� ���°� �ξ� �� ���� �� ����.

			// �������̽�: Expression
			class Expression
			{
			public:
				virtual ~Expression() {}
				virtual int Interpret(const std::unordered_map<std::string, int>& context) const = 0;
			};

			// ���� ǥ����
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
					return 0; // �⺻��
				}
			};

			// ���� ǥ����
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

			// ������ ǥ���� (����)
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

			// ������ ǥ���� (����)
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

			// �׽�Ʈ �Լ�
			void Test()
			{
				std::unordered_map<std::string, int> context = { {"x", 6}, {"y", 10} };

				// x + y * 2 ǥ���� ����
				std::unique_ptr<Expression> expression = std::make_unique<AddExpression>(
					std::make_unique<VariableExpression>("x"),
					std::make_unique<MultiplyExpression>(
						std::make_unique<VariableExpression>("y"),
						std::make_unique<NumberExpression>(5)));

				// �ؼ� �� ���
				std::cout << "Result: " << expression->Interpret(context) << std::endl;
			}

		}

		namespace Iterator
		{
			// ���� �����Ϳ� ������ �� �ִ� ����� ���� ������ ���� ���·� �Ϲ�ȭ���� �����ϴ� ����
			// STL ���� ���� ���ͷ����Ͱ� �� �̰Ŵ�.

			// ���ͷ����� �������̽�
			template <typename T>
			class Iterator
			{
			public:
				virtual ~Iterator() {}
				virtual bool HasNext() const = 0;
				virtual T Next() = 0;
			};

			// �����̳� �������̽�
			template <typename T>
			class Iterable
			{
			public:
				virtual ~Iterable() {}
				virtual Iterator<T>* CreateIterator() const = 0;
			};

			// ��� ����ü (�ܼ� ���� ����Ʈ ���)
			template <typename T>
			struct Node
			{
				T data;
				Node* next;
				Node(T value) : data(value), next(nullptr) {}
			};

			// �ܼ� ���� ����Ʈ ��� ���ͷ�����
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

			// �ܼ� ���� ����Ʈ �����̳�
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

			// �׽�Ʈ �Լ�
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
			// �ϳ��� ��Ʈ�� ��ü���� ��� ��ȣ�ۿ��� ���� ĸ��ȭ�ϴ� ��ü�� ���� ����.
			// Colleague ��� ��ü�� ������ ��ü�� ���� �޼����� �ְ� �޴´�.
			// �̸� ���� ��ü�� ���� ����� ����ȭ�Ǵ� ������ �ִ�.
			// �����ڿ� ��� �� �� ���ߵǾ ������ ��ü�� ������ �������� �� �ִ�.
			// ���� ��ü�� �����ϴ� ������� ������ �ϴµ�, �� �� �ϳ��� ��ü�� �����ؼ� �����ϱ� ����� ��� ���.
			// ���� ������ ¥�ٺ��� Room ������ �̰����� ó���ϴ� ��찡 ������, �װ��� �����Լ��� �ٲٸ� �����ڶ�� �� �� �ִ�.


			// ������ �������̽� (�� �ý���)
			class RoomMediator
			{
			public:
				virtual ~RoomMediator() {}
				virtual void Attack(class Character* attacker, const std::string& targetName, int damage) = 0;
			};

			// ĳ���� Ŭ���� (���� ������)
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

			// ���� �ý����� �����ϴ� Room Ŭ���� (������ ����)
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

					// ��� ã��
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

			// �׽�Ʈ �Լ�
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
			// ĸ��ȭ�� ����� �ʰ� ��ü�� ���¸� �����ؼ� ������ �� �ֵ��� ���ִ� ��.
			// ������ ���� ����� ũ�ų� ���簡 ����ϸ� �� ������ �ᵵ ȿ���� ��������.
			// Ŀ�ǵ� ������ ���� ������ �� ����

			// �÷��̾� ���¸� �����ϴ� �޸��� Ŭ����
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

			// �÷��̾� Ŭ���� (Originator)
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

				// ���� ���� ���� (Memento ����)
				std::shared_ptr<PlayerMemento> Save() const
				{
					return std::make_shared<PlayerMemento>(_hp, _mana, _level);
				}

				// ����� ���·� ����
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

				// ���� ������ ���� ��ȭ
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

			// ���¸� �����ϰ� �����ϴ� Caretaker Ŭ����
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

			// �׽�Ʈ �Լ�
			void Test()
			{
				Player player(100, 50, 1);
				SaveManager saveManager;

				player.ShowStatus();

				// ù ��° ����
				saveManager.SaveState(player.Save());

				// ���� ��ȭ
				player.TakeDamage(30);
				player.UseMana(20);
				player.LevelUp();

				player.ShowStatus();

				// �� ��° ����
				saveManager.SaveState(player.Save());

				// �߰� ��ȭ
				player.TakeDamage(50);
				player.UseMana(30);
				player.LevelUp();

				player.ShowStatus();

				// ���� ���·� ����
				player.Restore(saveManager.LoadState(0));
				player.ShowStatus();
			}
		}

		namespace Observer
		{
			// �ϴ�� �������� ���Ͽ� Ư�� ��ü�� ��ȭ�� �����Ͽ� �����ϰ� �ִ� ��ü���� �ڵ����� ��ȭ�ϵ��� �ϴ� ����
			// �������� ���ɻ� �����ؼ� Ư�� �̺�Ʈ�� �����ϰ� ������Ʈ ��ü�� ����� ���� �ִ�.
			// Change Manager ��� ���� �߰��ؼ� ��Ƽ���� ���(�ﰢ, ��ƺ�����)�� ���� �����ϴ�. ���⼭ Change Manager�� ������ ������ �������̴�.

			// ������ �������̽�
			class Observer
			{
			public:
				virtual ~Observer() {}
				virtual void OnNotify(const std::string& eventType, int value) = 0;
			};

			// ������Ʈ �������̽�
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

			// ������ ������ �ϴ� ChangeManager
			class ChangeManager
			{
			public:
				enum NotifyMode
				{
					IMMEDIATE, // �ﰢ �˸�
					BATCH      // ���� �ð� �� �ϰ� �˸�
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

			// �÷��̾� Ŭ���� (Subject)
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

			// UI Ŭ���� (Observer)
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

			// �׽�Ʈ �Լ�
			void Test()
			{
				ChangeManager changeManager(ChangeManager::BATCH);
				Player player(100, changeManager);
				UI ui;

				// UI�� Ư�� �̺�Ʈ�� ���
				player.AddObserver("HP_CHANGED", &ui);
				player.AddObserver("ITEM_ADDED", &ui);

				// ������ �Ա� (�˸��� �����ڿ� ���� ������)
				player.TakeDamage(20);
				player.TakeDamage(10);
				player.AddItem("Sword");
				player.AddItem("Potion");

				// �����ڰ� ��Ƶ� �̺�Ʈ�� �� ���� ó��
				std::cout << "\nFlushing notifications...\n";
				player.FlushNotifications();

				// �ﰢ �˸� ���� ����
				std::cout << "\nSwitching to IMMEDIATE mode...\n";
				changeManager.SetMode(ChangeManager::IMMEDIATE);

				player.TakeDamage(15);
				player.AddItem("Shield");
			}

		}

		namespace State
		{
			// ���� ���� ��ȭ�� ���� ��ü �ൿ ����� �ٲٴ� ����, ���°� �ٲ� ���� ��ü�� �ٲ� ��ó�� ���̰� �ȴ�.
			// ��ü�� ������ ���¿� ���� '��������' �ٲ� ���� �� �� �ִ�.
			// Enum ���� ���� ���� ���� ��ȭ�� ���� ������ ���� �ڵ带 �����ؼ� �ٲ�� �ϴµ�, �� ������ ���� ���� �ൿ�� �����ϴ� ���¸� ��ü�ϸ� �Ǿ ����.
			// �� �ɷ�ġ ���� ���¶�� �����ؼ� �����ϱ⺸�ٴ� ��� �����ϰ� �ִ� �͵� ���°� �� �� �ִٴ� ���� ��������.
	

			class Actor; // ���� ����
			class Monster;

			// ================================
			// CombatState �������̽� (��ø ����)
			// ================================
			class CombatState
			{
			public:
				virtual ~CombatState() {}
				virtual void ApplyEffect(Actor& actor) = 0;
				virtual void RemoveEffect(Actor& actor) = 0;
			};


			// ================================
			// ���� AI ���� (�� ���� �ϳ��� ����)
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


			// ���� ���� (���ݷ�/���� ����)
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
			// �÷��̾� (Character)
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

			// �������� ����
			class AggressiveState : public MonsterAIState
			{
			public:
				void HandleBehavior(Monster& monster) override;
			};

			// ����ȭ ���� (HP 30% ���� �� ����)
			class BerserkState : public MonsterAIState
			{
			public:
				void HandleBehavior(Monster& monster) override;
			};

			// ================================
			// ���� Ŭ���� (Monster)
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

			// AggressiveState ���� ����
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

			// BerserkState ���� ����
			void BerserkState::HandleBehavior(Monster& monster)
			{
				std::cout << monster.GetName() << " is in BERSERK mode and attacks with fury!\n";
			}

			// ================================
			// ���� �ùķ��̼� (���� ���� ����)
			// ================================
			void Battle(Character& player, Monster& enemy)
			{
				std::cout << "\n=== Battle Start: " << player.GetName() << " vs " << enemy.GetName() << " ===\n";

				player.ApplyCombatState("Buff", std::make_shared<BuffedState>(5, 2)); // �÷��̾� ���� ����
				enemy.ApplyCombatState("Debuff", std::make_shared<BuffedState>(-3, -1)); // ���� ����� ����

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
			// �׽�Ʈ �ڵ�
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
			// �� �˰��� ���� ���� �ش� �˰������ ���� �ٲ�鼭 ����� �� �ֵ��� �ϴ� ��.
			// �������ϰ��� ���̴� ��ü ������ ���� ���� ���ζ�� �Ѵ�. ������� ���� ����ϴ�.
			// C++ ���� ���� �������� ������ �������� ����� ���ٸ� ���ø��� �Ἥ �̰� ������ ���� �ְ�, �̷� ���� ���� Ŭ������ ���� �ʾƵ� �ȴ�.

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
					std::cout << "[����] ���� ����!\n";
				}
			};

			class RangeAttack : public IAttackStrategy
			{
			public:
				void Attack() override
				{
					std::cout << "[����] ���Ÿ� ����!\n";
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

			// ------------------- ���� Ŭ���� (���� + ���� ����) -------------------

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

			// ------------------- ���� ���� -------------------

			void IdleState::Enter(Monster& monster)
			{
				std::cout << "[����] ��� ���¿� �����ϴ�.\n";
			}

			void IdleState::Update(Monster& monster)
			{
				std::cout << "[����] �ֺ��� Ž�� ��...\n";
				monster.ChangeState(std::make_unique<ChasingState>());
			}

			void ChasingState::Enter(Monster& monster)
			{
				std::cout << "[����] �÷��̾ �ѱ� �����մϴ�!\n";
			}

			void ChasingState::Update(Monster& monster)
			{
				std::cout << "[����] �÷��̾ �߰� ��...\n";
				monster.ChangeState(std::make_unique<AttackingState>());
			}

			void AttackingState::Enter(Monster& monster)
			{
				std::cout << "[����] ���� �¼��� ����!\n";
			}

			void AttackingState::Update(Monster& monster)
			{
				std::cout << "[����] �÷��̾ �����մϴ�!\n";
				monster.ChangeState(std::make_unique<IdleState>());
			}

			// ------------------- �׽�Ʈ �ڵ� -------------------

			void Test()
			{
				std::cout << "=== ���� ���� ���� �׽�Ʈ ===\n";
				Monster monster(std::make_unique<MeleeAttack>());
				monster.Attack();

				monster.SetAttackStrategy(std::make_unique<RangeAttack>());
				monster.Attack();

				std::cout << "\n=== ���� ���� ���� �׽�Ʈ ===\n";
				monster.Update();
				monster.Update();
				monster.Update();
			}

		}

		namespace TemplateMethod
		{
			// �˰����� ���� Ʋ�� �����ΰ� Ư�� ���ۿ� ���� ���� ���� Ŭ������ �����ϴ� ��.
			// �̸� ���� ��ü���� Ʋ�� �����ϸ鼭 ���� ������ ������ �� �ִ�.
			// �� Ŭ�������� Ư�� ���۵��� �����Ǿ� �ִµ�, �������� �ٲ� ���� ����.

			class MonsterAI
			{
			public:
				virtual ~MonsterAI() = default;

				// ���ø� �޼��� (��ü AI �帧 ����)
				void RunAI()
				{
					DetectEnemy();
					MoveToTarget();
					Attack();  // ���� Ŭ�������� ����
					Retreat();
				}

			protected:
				void DetectEnemy()
				{
					std::cout << "[AI] ���� Ž���߽��ϴ�.\n";
				}

				void MoveToTarget()
				{
					std::cout << "[AI] ��ǥ�� ���� �̵� ��...\n";
				}

				void Retreat()
				{
					std::cout << "[AI] �����մϴ�.\n";
				}

				// ���� ����� ���� Ŭ�������� ����
				virtual void Attack() = 0;
			};

			class MeleeMonster : public MonsterAI
			{
			protected:
				void Attack() override
				{
					std::cout << "[Melee] ���� ����!\n";
				}
			};

			class RangeMonster : public MonsterAI
			{
			protected:
				void Attack() override
				{
					std::cout << "[Range] ���Ÿ� ����!\n";
				}
			};

			void Test()
			{
				std::cout << "=== ���� ���� AI ���� ===\n";
				std::unique_ptr<MonsterAI> melee = std::make_unique<MeleeMonster>();
				melee->RunAI();

				std::cout << "\n=== ���Ÿ� ���� AI ���� ===\n";
				std::unique_ptr<MonsterAI> range = std::make_unique<RangeMonster>();
				range->RunAI();
			}
		}

		namespace Visitor
		{
			// �� ��ü�� ���� ��ҵ鿡�� ����� ������, �ش� ��ҵ��� �������� �ʰ� �߰��� �� �ֵ��� ���ִ� ����. 
			// Visitor �߰��� �� ��ҿ��� ����� ���ο� ���� �߰���� ���� �ȴ�.
			// ��ü ���� Ư�� ��ü ���� ������ ���� �ٲ��� ���� ����, �پ��� ������ ��ü ���� ������ ����Ǿ�� �� ���� ����Ѵ�.
			
			// ����
			// Element ������ �߰����� �ʰ�, �ش� element�� �ʿ��� �������� ��� �߰��Ǿ�� �� ���� ���� ����.
			// �� ������ ���� ��Ӱ��谡 �ƴ� Ŭ������ ��ȸ�ϸ� ������ �۵���ų �� �ִ� ���� ����.
			// ��ȸ�ϸ鼭 ���� ���¸� �׾Ƶ־��� ������ ���� ����.
			
			// ����
			// ��ü ������ ���ԵǴ� ��ü(element) ������ �����Ѵٸ�, �����Ǵ� visitor�� ��� ����� ����ؼ� ����ϱ⿡ ���δ�.
			// ��ȸ�ϸ鼭 ������ �������� element�� ���� ���� ������ �������̽� �������־�� �ϴµ�, �̷� ���� ĸ��ȭ�� ��ȭ�� �� �ִ�.
			

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

			// ------------------- Entity �������̽� -------------------

			class IEntity
			{
			public:
				virtual ~IEntity() = default;
				virtual void Accept(IVisitor& visitor) = 0;
			};

			// ------------------- ��ü���� ���� ������Ʈ -------------------

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

			// ------------------- Visitor ���� (������) -------------------

			class RenderVisitor : public IVisitor
			{
			public:
				void Visit(Player& player) override
				{
					std::cout << "[Render] �÷��̾� " << player.name << " (HP: " << player.hp << ") ȭ�鿡 ǥ��\n";
				}

				void Visit(Monster& monster) override
				{
					std::cout << "[Render] ���� " << monster.type << " (HP: " << monster.hp << ") ȭ�鿡 ǥ��\n";
				}

				void Visit(Shop& shop) override
				{
					std::cout << "[Render] ���� '" << shop.shopName << "' ������ ǥ��\n";
				}
			};

			// ------------------- Visitor ���� (��ȣ�ۿ�) -------------------

			class InteractionVisitor : public IVisitor
			{
			public:
				void Visit(Player& player) override
				{
					std::cout << "[Interaction] �÷��̾� " << player.name << "��(��) ��ȣ�ۿ��� �� ����.\n";
				}

				void Visit(Monster& monster) override
				{
					std::cout << "[Interaction] ���� " << monster.type << "���� ���� ����!\n";
				}

				void Visit(Shop& shop) override
				{
					std::cout << "[Interaction] ���� '" << shop.shopName << "'���� ������ ���� ����!\n";
				}
			};

			// ------------------- ���� �� Ŭ���� -------------------

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

			// ------------------- �׽�Ʈ �ڵ� -------------------

			void Test()
			{
				GameMap gameMap;

				gameMap.AddEntity(std::make_unique<Player>("Hero", 100));
				gameMap.AddEntity(std::make_unique<Monster>("Goblin", 50));
				gameMap.AddEntity(std::make_unique<Shop>("Potion Shop"));

				RenderVisitor renderer;
				InteractionVisitor interactor;

				std::cout << "=== �� ������ ===\n";
				gameMap.ApplyVisitor(renderer);

				std::cout << "\n=== �÷��̾� ��ȣ�ۿ� üũ ===\n";
				gameMap.ApplyVisitor(interactor);
			}

		}
	}
}