#include<Windows.h>
#include<iostream>
#pragma comment(lib,"Winmm.lib")
#include<mmsystem.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

/// Настройки меню
bool menu = true;
bool game = false;

/// Здоровье
int health = 10;

/// Статистика
int score = 0;
int asteroid = 0;
int torpedo = 0;
int enemy = 0;

/// Космос
const int row = 50;
const int col = 90;
char space[row][col];
char vacuum = ' ';

/// Космический корабль
char spaceship = '&';
int spaceshipRow = 25;
int spaceshipCol = 35;

/// Снаряд
const int projectileCount = 15;
int index = 0;
int magazine = 15;
struct Projectile
{
	char projectile = '^';
	bool isActive = false;
	int projectileRow;
	int projectileCol;
};
Projectile* P_projectile;

/// Астероид
const int asteroidCount = 30;
struct Asteroid
{
	char asteroid = '*';
	bool isDestroyed = true;
	int asteroidRow;
	int asteroidCol;
};
Asteroid* P_asteroid;

/// Торпеда
const int torpedoCount = 25;
struct Torpedo
{
	char torpedo = '!';
	bool isDestroyed = true;
	int torpedoRow;
	int torpedoCol;
};
Torpedo* P_torpedo;

/// Вражеский корабль
const int enemyCount = 20;
struct Enemy
{
	char enemy = '$';
	bool isDestroyed = true;
	bool didShoot = false;
	int enemyRow;
	int enemyCol;
};
Enemy* P_enemy;

/// Снаряд врага
const int missileCount = enemyCount;
struct Missile
{
	char missile = 248;
	bool isActive = false;
	int missileRow;
	int missileCol;
};
Missile* P_missile;

/// Столкновение астероида
void CollisionAsteroid(Asteroid* P_asteroid, Projectile* P_projectile)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == false)
		{
			if (P_asteroid[i].asteroidRow == spaceshipRow && P_asteroid[i].asteroidCol == spaceshipCol)
			{
				health--;
				P_asteroid[i].isDestroyed = true;
			}
		}

		for (int j = 0; j < projectileCount; j++)
		{
			if (P_asteroid[i].isDestroyed == false && P_projectile[j].isActive == true)
			{
				if (P_asteroid[i].asteroidRow == P_projectile[j].projectileRow && P_asteroid[i].asteroidCol == P_projectile[j].projectileCol)
				{
					score += 10;
					asteroid++;
					P_asteroid[i].isDestroyed = true;
					P_projectile[j].isActive = false;
					if (j == projectileCount - 1)
					{
						P_projectile[i].isActive = false;
						index = 0;
						magazine = 15;
					}
				}
			}
		}
	}
}

/// Столкновение торпеды
void CollisionTorpedo(Torpedo* P_torpedo, Projectile* P_projectile)
{
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == false)
		{
			if ((P_torpedo[i].torpedoRow - 1 == spaceshipRow && P_torpedo[i].torpedoCol == spaceshipCol) || (P_torpedo[i].torpedoRow == spaceshipRow && P_torpedo[i].torpedoCol == spaceshipCol))
			{
				health--;
				P_torpedo[i].isDestroyed = true;
			}
		}

		for (int j = 0; j < projectileCount; j++)
		{
			if (P_torpedo[i].isDestroyed == false && P_projectile[j].isActive == true)
			{
				if ((P_torpedo[i].torpedoRow == P_projectile[j].projectileRow && P_torpedo[i].torpedoCol == P_projectile[j].projectileCol) || (P_torpedo[i].torpedoRow - 1 == P_projectile[j].projectileRow && P_torpedo[i].torpedoCol == P_projectile[j].projectileCol))
				{
					score += 20;
					torpedo++;
					P_torpedo[i].isDestroyed = true;
					P_projectile[j].isActive = false;
					if (j == projectileCount - 1)
					{
						P_projectile[i].isActive = false;
						index = 0;
						magazine = 15;
					}
				}
			}
		}
	}
}

/// Столкновение с врагом
void CollisionEnemy(Enemy* P_enemy, Projectile* P_projectile)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (P_enemy[i].isDestroyed == false)
		{
			if (P_enemy[i].enemyRow == spaceshipRow && P_enemy[i].enemyCol == spaceshipCol)
			{
				health--;
				P_enemy[i].isDestroyed = true;
			}
		}

		for (int j = 0; j < projectileCount; j++)
		{
			if (P_enemy[i].isDestroyed == false && P_projectile[j].isActive == true)
			{
				if (P_enemy[i].enemyRow == P_projectile[j].projectileRow && P_enemy[i].enemyCol == P_projectile[j].projectileCol)
				{
					score += 30;
					enemy++;
					P_enemy[i].isDestroyed = true;
					P_projectile[j].isActive = false;
					if (j == projectileCount - 1)
					{
						P_projectile[i].isActive = false;
						index = 0;
						magazine = 15;
					}
				}
			}
		}
	}
}

/// Столкновение со снарядом врага
void CollisionMissile(Missile* P_missile)
{
	for (int i = 0; i < missileCount; i++)
	{
		if (P_missile[i].isActive == true)
		{
			if ((P_missile[i].missileRow - 1 == spaceshipRow && P_missile[i].missileCol == spaceshipCol) || (P_missile[i].missileRow == spaceshipRow && P_missile[i].missileCol == spaceshipCol))
			{
				health--;
				P_missile[i].isActive = false;
			}
		}
	}
}

/// Столкновение
void Collision()
{
	CollisionAsteroid(P_asteroid, P_projectile);
	CollisionTorpedo(P_torpedo, P_projectile);
	CollisionEnemy(P_enemy, P_projectile);
	CollisionMissile(P_missile);
}

/// Слздание снаряда врага
void SpawnMissile(Missile* P_missile)
{
	for (int i = 0; i < missileCount; i++)
	{
		if (P_enemy[i].didShoot == true)
		{
			P_enemy[i].didShoot = false;
			P_missile[i].isActive = true;
			P_missile[i].missileRow = P_enemy[i].enemyRow + 1;
			P_missile[i].missileCol = P_enemy[i].enemyCol;
		}
	}
}

/// Показ снаряда врага
void InputMissile(Missile* P_missile)
{
	for (int i = 0; i < missileCount; i++)
	{
		if (P_missile[i].isActive == true)
		{
			space[P_missile[i].missileRow][P_missile[i].missileCol] = P_missile[i].missile;
		}
	}
}

/// Движение снаряда врага
void MissileMovement(Missile* P_missile)
{
	for (int i = 0; i < missileCount; i++)
	{
		if (P_missile[i].isActive == true)
		{
			P_missile[i].missileRow += 2;
			if (P_missile[i].missileRow > row - 1)
			{
				P_missile[i].isActive = false;
			}
		}
	}
	Collision();
}

/// Создание вражеского корабля
void SpawnEnemy(Enemy* P_enemy)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (P_enemy[i].isDestroyed == true)
		{
			int rCol = rand() % 2;
			P_enemy[i].isDestroyed = false;
			P_enemy[i].enemyRow = rand() % 15;
			if (rCol == 0)
			{
				P_enemy[i].enemyCol = 0;
			}
			else if (rCol == 1)
			{
				P_enemy[i].enemyCol = 69;
			}
		}
	}
}
void CheckRandomEnemy(Enemy* P_enemy)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (P_enemy[i].isDestroyed == false)
		{
			for (int j = i + 1; j < enemyCount; j++)
			{
				if (P_enemy[i].enemyRow == P_enemy[j].enemyRow && P_enemy[i].enemyCol == P_enemy[j].enemyCol)
				{
					P_enemy[j].enemyRow = rand() % 15;
					P_enemy[j].enemyCol = 0;
				}
			}
		}
	}
}

/// Показ вражеского корабля
void InputEnemy(Enemy* P_enemy)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (P_enemy[i].isDestroyed == false)
		{
			space[P_enemy[i].enemyRow][P_enemy[i].enemyCol] = P_enemy[i].enemy;
		}
	}
}

/// Движение вражеского корабля
void EnemyMovement(Enemy* P_enemy)
{
	for (int i = 0; i < enemyCount; i++)
	{
		int movement = 1 + rand() % 5;
		if (P_enemy[i].isDestroyed == false)
		{
			if (movement == 1)
			{
				P_enemy[i].enemyRow++;
			}
			else if (movement == 2)
			{
				P_enemy[i].enemyRow--;
			}
			else if (movement == 3)
			{
				P_enemy[i].enemyCol--;
			}
			else if (movement == 4)
			{
				P_enemy[i].enemyCol++;
			}
			else if (movement == 5 && P_enemy[i].didShoot == false && P_missile[i].isActive == false)
			{
				P_enemy[i].didShoot = true;
			}

			if (P_enemy[i].enemyRow<0 || P_enemy[i].enemyRow>row - 1 || P_enemy[i].enemyCol<0 || P_enemy[i].enemyCol>col - 1)
			{
				P_enemy[i].isDestroyed = true;
			}
		}
	}
	Collision();
}

/// Создание торпеды
void SpawnTorpedo(Torpedo* P_torpedo)
{
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == true)
		{
			P_torpedo[i].isDestroyed = false;
			P_torpedo[i].torpedoRow = 0;
			P_torpedo[i].torpedoCol = rand() % 70;
		}
	}
}
void CheckRandomTorpedo(Torpedo* P_torpedo)
{
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == false)
		{
			for (int j = i + 1; j < torpedoCount; j++)
			{
				if (P_torpedo[i].torpedoCol == P_torpedo[j].torpedoCol)
				{
					P_torpedo[j].torpedoCol = rand() % 70;
				}
			}
		}
	}
}

/// Показ торпед
void InputTorpedo(Torpedo* P_torpedo)
{
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == false)
		{
			space[P_torpedo[i].torpedoRow][P_torpedo[i].torpedoCol] = P_torpedo[i].torpedo;
		}
	}
}

/// Движение торпед
void TorpedoMovement(Torpedo* P_torpedo)
{
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == false)
		{
			P_torpedo[i].torpedoRow += 2;
			if (P_torpedo[i].torpedoRow > row - 1)
			{
				P_torpedo[i].isDestroyed = true;
			}
		}
	}
	Collision();
}

/// Создание астероидов
void SpawnAsteroids(Asteroid* P_asteroid)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == true)
		{
			P_asteroid[i].isDestroyed = false;
			P_asteroid[i].asteroidRow = 0;
			P_asteroid[i].asteroidCol = rand() % 70;
		}
	}
}
void CheckRandomAsteroid(Asteroid* P_asteroid)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == false)
		{
			for (int j = i + 1; j < asteroidCount; j++)
			{
				if (P_asteroid[i].asteroidCol == P_asteroid[j].asteroidCol)
				{
					P_asteroid[j].asteroidCol = rand() % 70;
				}
			}
		}
	}
}

/// Показ астероидов
void InputAsteroids(Asteroid* P_asteroid)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == false)
		{
			space[P_asteroid[i].asteroidRow][P_asteroid[i].asteroidCol] = P_asteroid[i].asteroid;
		}
	}
}

/// Движение астероидов
void AsteroidMovement(Asteroid* P_asteroid)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == false)
		{
			P_asteroid[i].asteroidRow++;
			if (P_asteroid[i].asteroidRow > row - 1)
			{
				P_asteroid[i].isDestroyed = true;
			}
		}
	}
	Collision();
}

/// Проверка случайностей
void CheckRandom(Asteroid* P_asteroid, Torpedo* P_torpedo, Enemy* P_enemy)
{
	CheckRandomAsteroid(P_asteroid);
	CheckRandomTorpedo(P_torpedo);
	CheckRandomEnemy(P_enemy);
}



/// Выстрел
void Shot(Projectile* P_projectile)
{
	if (index != 15)
	{
		P_projectile[index].isActive = true;
		P_projectile[index].projectileRow = spaceshipRow - 1;
		P_projectile[index].projectileCol = spaceshipCol;
		index++;
		magazine--;
	}
}

/// Показ снаряда
void InputProjectile(Projectile* P_projectile)
{
	for (int i = 0; i < projectileCount; i++)
	{
		if (P_projectile[i].isActive == true)
		{
			space[P_projectile[i].projectileRow][P_projectile[i].projectileCol] = P_projectile[i].projectile;
		}
	}
}

/// Движение снаряда
void ProjectileMovement(Projectile* P_projectile)
{
	for (int i = 0; i < projectileCount; i++)
	{
		if (P_projectile[i].isActive == true)
		{
			P_projectile[i].projectileRow--;
			if (P_projectile[i].projectileRow < 0)
			{
				if (i == projectileCount - 1)
				{
					P_projectile[i].isActive = false;
					index = 0;
					magazine = 15;
				}
				else
				{
					P_projectile[i].isActive = false;
				}
			}
		}
	}
	Collision();
}



///Настройки по умолчанию
void DefaultSettings()
{
	for (int i = 0; i < projectileCount; i++)
	{
		if (P_projectile[i].isActive == true)
		{
			P_projectile[i].isActive = false;
		}
	}
	for (int i = 0; i < asteroidCount; i++)
	{
		if (P_asteroid[i].isDestroyed == false)
		{
			P_asteroid[i].isDestroyed = true;
		}
	}
	for (int i = 0; i < torpedoCount; i++)
	{
		if (P_torpedo[i].isDestroyed == false)
		{
			P_torpedo[i].isDestroyed = true;
		}
	}
	for (int i = 0; i < enemyCount; i++)
	{
		if (P_enemy[i].isDestroyed == false)
		{
			P_enemy[i].isDestroyed = true;
		}
	}
	for (int i = 0; i < missileCount; i++)
	{
		if (P_missile[i].isActive == true)
		{
			P_missile[i].isActive = false;
		}
	}
	spaceshipRow = 25;
	spaceshipCol = 35;
	health = 10;
	magazine = 15;
	index = 0;
	score = 0;
	asteroid = 0;
	torpedo = 0;
	enemy = 0;
}

/// Создание космоса
void createSpace()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			space[i][j] = vacuum;
		}
	}
	Collision();
}

/// Инициализация корабля
void SpawnSpaceship()
{
	space[spaceshipRow][spaceshipCol] = spaceship;
}

/// Показ космоса
void showSpace()
{
	cout << "\n\n\n\n\n" << endl;
	for (int i = 0; i < row; i++)
	{
		cout << "\t\t\t\t\t\t\b\b\b\b\b";
		for (int j = 0; j < col; j++)
		{
			cout << space[i][j] << " ";
		}
		cout << endl;
	}
	Sleep(0.4);
	system("cls");
}

/// Управление кораблем
void spaceshipControl()
{
	if (_kbhit())
	{
		char movement = _getch();
		switch (movement)
		{
		case 27:
			game = false;
			break;
		case'w':
			if (spaceshipRow > 0)
			{
				spaceshipRow--;
			}
			break;
		case's':
			if (spaceshipRow < row - 1)
			{
				spaceshipRow++;
			}
			break;
		case'a':
			if (spaceshipCol > 0)
			{
				spaceshipCol--;
			}
			break;
		case'd':
			if (spaceshipCol < col - 1)
			{
				spaceshipCol++;
			}
			break;
		case 32:
			Shot(P_projectile);
			break;
		}
	}
}



/// Проверка конца игры
void GameOver()
{
	if (health == 0)
	{
		game = false;
	}
	if (game == false)
	{
		PlaySound(TEXT("GameOver"), NULL, SND_FILENAME | SND_ASYNC);
		cout << R"(
 _______  _______  _______  _______    _______           _______  _______ 
(  ____ \(  ___  )(       )(  ____ \  (  ___  )|\     /|(  ____ \(  ____ )
| (    \/| (   ) || () () || (    \/  | (   ) || )   ( || (    \/| (    )|
| |      | (___) || || || || (__      | |   | || |   | || (__    | (____)|
| | ____ |  ___  || |(_)| ||  __)     | |   | |( (   ) )|  __)   |     __)
| | \_  )| (   ) || |   | || (        | |   | | \ \_/ / | (      | (\ (   
| (___) || )   ( || )   ( || (____/\  | (___) |  \   /  | (____/\| ) \ \__
(_______)|/     \||/     \|(_______/  (_______)   \_/   (_______/|/   \__/
                                                                          
)" << endl;
		Sleep(2000);
		system("cls");
		DefaultSettings();
	}
}

void Interface()
{
	cout << "Healt: " << health << endl;
	cout << "Score: " << score << endl;
	cout << "Asteroid: " << asteroid << endl;
	cout << "Torpedo: " << torpedo << endl;
	cout << "Enemy: " << enemy << endl;
	cout << "\n\nMagazine: " << magazine;
}


void Menu()
{
	cout << R"(
   __    _______  _______  _______  _______             _______  _______  _ 
  /__\  (  ____ )(  ___  )(  ____ \(  ____ \  |\     /|(  ___  )(  ____ )( )
 ( \/ ) | (    )|| (   ) || (    \/| (    \/  | )   ( || (   ) || (    )|| |
  \  /  | (____)|| (___) || |      | (__      | | _ | || (___) || (____)|| |
  /  \/\|  _____)|  ___  || |      |  __)     | |( )| ||  ___  ||     __)| |
 / /\  /| (      | (   ) || |      | (        | || || || (   ) || (\ (   (_)
(  \/  \| )      | )   ( || (____/\| (____/\  | () () || )   ( || ) \ \__ _ 
 \___/\/|/       |/     \|(_______/(_______/  (_______)|/     \||/   \__/(_)
                                                                            
)" << endl;

	cout << R"(
______                    _____      _              _                _                            _____ _____ _____   _                    _ _   
| ___ \                  |  ___|    | |            | |              | |                          |  ___/  ___/  __ \ | |                  (_) |  
| |_/ / __ ___  ___ ___  | |__ _ __ | |_ ___ _ __  | |_ ___    _ __ | | __ _ _   _    ___  _ __  | |__ \ `--.| /  \/ | |_ ___     _____  ___| |_ 
|  __/ '__/ _ \/ __/ __| |  __| '_ \| __/ _ \ '__| | __/ _ \  | '_ \| |/ _` | | | |  / _ \| '__| |  __| `--. \ |     | __/ _ \   / _ \ \/ / | __|
| |  | | |  __/\__ \__ \ | |__| | | | ||  __/ |    | || (_) | | |_) | | (_| | |_| | | (_) | |    | |___/\__/ / \__/\ | || (_) | |  __/>  <| | |_ 
\_|  |_|  \___||___/___/ \____/_| |_|\__\___|_|     \__\___/  | .__/|_|\__,_|\__, |  \___/|_|    \____/\____/ \____/  \__\___/   \___/_/\_\_|\__|
                                                              | |             __/ |                                                              
                                                              |_|            |___/                                                               
)" << endl;

}



int main()
{
	Projectile projectile[projectileCount];
	P_projectile = projectile;

	Asteroid asteroid[asteroidCount];
	P_asteroid = asteroid;

	Torpedo torpedo[torpedoCount];
	P_torpedo = torpedo;

	Enemy enemy[enemyCount];
	P_enemy = enemy;

	Missile missile[missileCount];
	P_missile = missile;

	while (menu)
	{
		PlaySound(TEXT("Solar Fields - Insum.wav"), NULL, SND_FILENAME | SND_ASYNC);
		Menu();
		char operation = _getch();
		if (operation == 27)
		{
			menu = false;
			system("cls");
		}
		else if (operation == 13)
		{
			PlaySound(TEXT("Mega Drive - Converter.wav"), NULL, SND_FILENAME | SND_ASYNC);
			game = true;
			while (game)
			{
				srand(time(NULL));
				createSpace();

				InputProjectile(P_projectile);

				SpawnAsteroids(P_asteroid);
				SpawnTorpedo(P_torpedo);
				SpawnEnemy(P_enemy);
				SpawnMissile(P_missile);

				CheckRandom(P_asteroid, P_torpedo, P_enemy);


				InputAsteroids(P_asteroid);
				InputTorpedo(P_torpedo);
				InputEnemy(P_enemy);
				InputMissile(P_missile);



				ProjectileMovement(P_projectile);
				AsteroidMovement(P_asteroid);
				TorpedoMovement(P_torpedo);
				EnemyMovement(P_enemy);
				MissileMovement(P_missile);


				SpawnSpaceship();

				Interface();
				showSpace();

				spaceshipControl();

				GameOver();
			}
		}
		else
		{
			system("cls");
		}
	}
}