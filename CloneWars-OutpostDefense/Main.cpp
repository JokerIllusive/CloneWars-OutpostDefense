#include <iostream>
#include <string>

using namespace std;

const int MAP_WIDTH = 8;
const int MAP_HEIGHT = 5;
// =========================
// Status Effect Bitflags
// =========================
const unsigned int STATUS_NONE = 0;
const unsigned int STATUS_SUPPRESSED = 1 << 0;
const unsigned int STATUS_SHIELDED = 1 << 1;
const unsigned int STATUS_STUNNED = 1 << 2;
const unsigned int STATUS_BURNING = 1 << 3;
const unsigned int STATUS_POISONED = 1 << 4;
// =========================
// Base Unit Class
// =========================
class Unit
{
protected:
	string name;
	int health;
	int attackPower;
	unsigned int statusEffects;
	int x;
	int y;

public:
	Unit(string newName, int newHealth, int newAttack, int startX, int startY)
	{
		name = newName;
		health = newHealth;
		attackPower = newAttack;
		statusEffects = STATUS_NONE;
		x = startX;
		y = startY;
	}

	virtual ~Unit()
	{
	}

	// =========================
	// Basic Getters
	// =========================
	string GetName() const
	{
		return name;
	}

	int GetHealth() const
	{
		return health;
	}

	bool IsAlive() const
	{
		return health > 0;
	}

	int GetX() const
	{
		return x;
	}

	int GetY() const
	{
		return y;
	}

	// =========================
	// Status Effect Functions
	// =========================
	void addStatus(unsigned int status)
	{
		statusEffects |= status;
	}

	void RemoveStatus(unsigned int status)
	{
		statusEffects &= ~status;
	}

	bool HasStatus(unsigned int status) const
	{
		return (statusEffects & status) != 0;
	}

	void PrintStatusEffects() const
	{
		cout << "Status Effects: ";

		bool hasAny = false;

		if (HasStatus(STATUS_SUPPRESSED))
		{
			cout << "[SUPPRESSED] ";
			hasAny = true;
		}

		if (HasStatus(STATUS_SHIELDED))
		{
			cout << "[SHIELDED] ";
			hasAny = true;
		}

		if (HasStatus(STATUS_STUNNED))
		{
			cout << "[STUNNED] ";
			hasAny = true;
		}

		if (HasStatus(STATUS_BURNING))
		{
			cout << "[BURNING]";
			hasAny = true;
		}
		if (HasStatus(STATUS_POISONED))
		{
			cout << "[POISONED]";
			hasAny = true;
		}

		if (!hasAny)
		{
			cout << "NONE";
		}
		
		cout << endl;
	}

	// =========================
	// Rendering Functions
	// =========================
	virtual char GetMapSymbol() const
	{
		return '?';
	}

	// =========================
	// Movement Functions
	// =========================
	void Move(int moveX, int moveY)
	{
		int newX = x + moveX;
		int newY = y + moveY;

		if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
		{
			x = newX;
			y = newY;
		}
		else
		{
			cout << name << " cannot move outside the battlefield!\n";
		}
	}

	// =========================
    // Combat Functions
    // =========================
	virtual void Attack(Unit& target)
	{
		if (!IsAlive())
		{
			return;
		}

		if (HasStatus(STATUS_STUNNED))
		{
			cout << name << " is stunned and cannot attack!\n";

			RemoveStatus(STATUS_STUNNED);

			return;
		}

		int finalDamage = attackPower;

		if (HasStatus(STATUS_SUPPRESSED))
		{
			finalDamage /= 2;

			cout << name << " is suppressed! Damage reduced.\n";

			RemoveStatus(STATUS_SUPPRESSED);
		}

		cout << name << " attacks " << target.GetName() << "!\n";

		target.TakeDamage(finalDamage);
	}

	virtual void TakeDamage(int damage)
	{
		if (HasStatus(STATUS_SHIELDED))
		{
			damage /= 2;

			cout << name << "'s shield absorbed some damage!\n";
		}

		health -= damage;

		if (health < 0)
		{
			health = 0;
		}

		cout <<	name << " takes " << damage << " damage.\n";

	}
			void ApplyEndTurnEffects()
			{
				if (HasStatus(STATUS_BURNING))
				{
					cout << name << " suffers burning damage!\n";
					health -= 2;
				}

				if (HasStatus(STATUS_POISONED))
				{
					cout << name << " suffers poison damage!\n";
					health -= 1;
				}

				if (health < 0)
				{
					health = 0;
				}
			}

			virtual void PrintUnitType() const
			{
				cout << "Unit";
			}
		};

		// =========================
		// Clone Trooper Class
		// =========================
		class CloneTrooper : public Unit
		{
		public:
			CloneTrooper(string newName, int startX, int startY)
				: Unit(newName, 30, 8, startX, startY)
			{
			}
		

			void Shield()
			{
				cout << name << " activates shield protection!\n";
				addStatus(STATUS_SHIELDED);
			}

			void SuppressedTarget(Unit& target)
			{
				cout << name << " suppressed " << target.GetName() << "!\n";

				target.addStatus(STATUS_SUPPRESSED);
			}

			void PrintUnitType() const
			{
				cout << "Clone Trooper";
			}

			char GetMapSymbol() const
			{
				return 'C';
			}
		};

		// =========================
		// Battle Droid Class
		// =========================

		class BattleDroid : public Unit
		{
		public:
			BattleDroid(string newName, int startX, int startY)
				: Unit(newName, 20, 6, startX, startY)
			{
			}
				

			void SuppressTarget(Unit& target)
			{
				cout << name << " suppresses " << target.GetName() << "!\n";

				target.addStatus(STATUS_SUPPRESSED);
			}

			void PrintUnitType() const
			{
				cout << "Battle Droid";
			}

			char GetMapSymbol() const
			{
				return 'D';
			}
		};
		// =========================
		// Helper Functions
		// =========================
		void PrintUnitInfo(const Unit& unit)
		{
			unit.PrintUnitType();

			cout << " | "
				<< unit.GetName()
				<< " | HP: "
				<< unit.GetHealth()
				<< endl;

			unit.PrintStatusEffects();

			cout << endl;
		}

		int GetDistance(const Unit& a, const Unit& b)
		{
			int xDistance = a.GetX() - b.GetX();
			int yDistance = a.GetY() - b.GetY();

			if (xDistance < 0)
			{
				xDistance *= -1;
			}

			if (yDistance < 0)
			{
				yDistance *= -1;
			}

			return xDistance + yDistance;
		}

	void DrawMap(const CloneTrooper& clone, const BattleDroid& droid)
		{
			cout << "\nBattlefield:\n\n";

			for (int row = 0; row < MAP_HEIGHT; row++)
			{
				for (int col = 0; col < MAP_WIDTH; col++)
				{
					if (clone.IsAlive() && clone.GetX() == col && clone.GetY() == row)
					{
						cout << clone.GetMapSymbol() << " ";
					}
					else if (droid.IsAlive() && droid.GetX() == col && droid.GetY() == row)
					{
						cout << droid.GetMapSymbol() << " ";
					}
					else
					{
						cout << ". ";
					}
				}

				cout << endl;
			}
			cout << "\nC = Clone | D = Droid\n";
		}

		void MoveDroidTowardClone(BattleDroid& droid, const CloneTrooper& clone)
		{
			if (clone.GetX() > droid.GetX())
			{
				droid.Move(1,0);
			}
			else if (clone.GetX() < droid.GetX())
			{
				droid.Move(-1, 0);
			}
			else if (clone.GetY() > droid.GetY())
			{
				droid.Move(0, 1);
			}
			else if (clone.GetY() < droid.GetY())
			{
				droid.Move(0, -1);
			}
		}

		// =========================
		// Main Game
		// =========================
		int main()
		{
			CloneTrooper clone("CT-7567 Rex",1, 1);
			BattleDroid droid("B1 Battle Droid",6, 3);

			cout << "=================================\n";
			cout << " CLONE WARS TACTICAL PROTOTYPE\n";
			cout << "Version 1 \n";
			cout << "=================================\n";

			cout << "A lone Clone Trooper stands guard at an outpost on a remote planet. Suddenly, a B1 Battle Droid appears, intent on destroying the outpost. The Clone must defend against the droid's assault and survive until reinforcements arrive.\n";
			cout << "Move the clone using WASD keys. Choose actions each turn to defeat the droid!\n";
			cout << "Melee attacks require being adjacent to the enemy!\n";
			cout << "Shielding will last until the next turn.\n";
			cout << "Suppression reduces the target's damage for one turn.\n";
			cout << "Status effects are applied at the end of each turn, and will be displayed under each unit!\n";
			cout << "Press Enter to begin the battle...\n";

			int turn = 1;

			while (clone.IsAlive() && droid.IsAlive())
			{
				cout << "\n=================================\n";
				cout << " TURN " << turn << endl;
				cout << "=================================\n\n";

				DrawMap(clone, droid);
				PrintUnitInfo(clone);
				PrintUnitInfo(droid);

				// =========================
				// Player Turn
				// =========================
				cout << "___ Clone Turn ___\n";
				cout << "WASD = Move one tile\n";
				cout << "1. Melee Attack: range 1\n";
				cout << "2. Shield: Halves incoming damage\n";
				cout << "3. Suppress Fire: range 3, halves enemy damage\n";

				string input;
				cin >> input;

				if (input.length() != 1)
				{
					cout << "Enter one command only!\n";
					continue;
				}

				char choice = input[0];

				if (choice == '1')
					if (GetDistance(clone, droid) <= 1)
					{
						clone.Attack(droid);
					}
					else
					{
						cout << "Target is out of range!\n";
					}
				else if (choice == '2')
				{
					clone.Shield();
				}
				else if (choice == '3')
					if (GetDistance(clone, droid) <= 3)
					{
						clone.SuppressedTarget(droid);
					}
					else
					{
						cout << "Target is out of range!\n";
					}
				else
				{
					cout << "Invalid choice!\n";
				}

				if (!droid.IsAlive())
				{
					break;
				}

				// =========================
				// WASD Movement
				// =========================
				if (choice == 'w' || choice == 'W')
				{
					clone.Move(0, -1);
				}

				if (choice == 'a' || choice == 'A')
				{
					clone.Move(-1, 0);
				}

				if (choice == 's' || choice == 'S')
				{
					clone.Move(0, 1);
				}

				if (choice == 'd' || choice == 'D')
				{
					clone.Move(1, 0);
				}

				// =========================
				// Droid Turn
				// =========================
				cout << "\n--- Droid Turn ---\n";

				if (GetDistance(clone, droid) <= 1)
				{
					int aiChoice = rand() % 2;

					if (aiChoice == 0)
					{
						droid.Attack(clone);
					}
					else
					{
						droid.SuppressTarget(clone);
					}
				}
				
				else
				{
					cout << droid.GetName() << " Advances towards the clone.\n";
					MoveDroidTowardClone(droid, clone);
				}
				

				// =========================
				// End Turn Effects
				// =========================
				clone.ApplyEndTurnEffects();
				droid.ApplyEndTurnEffects();

				turn++;
			}

			// =========================
			// End Game
			// =========================
			cout << "\n=================================\n";

			if (clone.IsAlive())
			{
				cout << "Victory! The Clone Trooper has defeated the Battle Droid!\n";
			}
			else
			{
				cout << "Defeat! The Battle Droid has defeated the Clone Trooper!\n";
			}

			cout << "=================================\n";

			return 0;
		
}
