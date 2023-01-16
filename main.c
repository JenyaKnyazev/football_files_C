#define _CRT_SECURE_NO_WARNINGS
#define TEAM_NUM 18
#define PLAYERS_NUM 26
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
	int id;
	char name[20];
	int score;
	int redCards;
	int numOfGames;
	int goals;
}team;
typedef struct {
	int id;
	int goals;
	int redcart;
	int arr_of_players_goals[PLAYERS_NUM];
}game;
team* teamArr[TEAM_NUM];
int gamesMat[TEAM_NUM][PLAYERS_NUM];
int is_read_file_first = 0;
int num_of_teams=0;
int num_of_games = 0;
void init() {
	int i,j;
	for (i = 0; i < TEAM_NUM; i++) {
		teamArr[i] = NULL;
		for(j=0;j<PLAYERS_NUM;j++)
			gamesMat[i][j] = 0;
	}
}
void add_to_team_arr(team* t) {
	int i;
	for (i = 0; i < TEAM_NUM; i++)
		if (teamArr[i] == NULL) {
			teamArr[i] = t;
			break;
		}
}
team* init_team(int id,char* name) {
	team* element = (team*)malloc(sizeof(team));
	element->id = id;
	strcpy(element->name,name);
	element->numOfGames = 0;
	element->redCards = 0;
	element->score = 0;
	element->goals = 0;
	return element;
}
team* copy_team(team* t) {
	team* ele=NULL;
	if (t != NULL) {
		ele = (team*)malloc(sizeof(team));
		ele->goals = t->goals;
		ele->id = t->id;
		strcpy(ele->name, t->name);
		ele->numOfGames = t->numOfGames;
		ele->score = t->score;
		ele->redCards = t->redCards;
	}
	return ele;
}
team** get_copy_team_arr() {
	int i;
	team** t = (team**)malloc(sizeof(team*) * 18);
	for (i = 0; i < 18; i++) 
		t[i] = copy_team(teamArr[i]);
	return t;
}
void free_copy_of_team_arr(team** arr) {
	int i;
	for(i=0;i<18;i++)
		if (arr[i] != NULL) {
			free(arr[i]);
			arr[i] = NULL;
		}
	free(arr);
}
void free_team_arr() {
	int i;
	for (i = 0; i < 18; i++)
		if (teamArr[i] != NULL)
			free(teamArr[i]);
}
int readTeamFile(team** teamArr) {
	FILE* f;
	int i;
	f = fopen("./teams.txt", "r");
	if (f == NULL)
		return -1;
	is_read_file_first = 1;
	init();
	char name[20];
	int id;
	int count = 0;
	while (fscanf(f, "%d%s", &id, name) == 2&&count<18) {
		add_to_team_arr(init_team(id, name));
		count++;
	}
	fclose(f);
	num_of_teams = count;
	return count;
}
int getPos(team** teamArr, int teamNum, int numOfTeams) {
	int i;
	for (i = 0; i < numOfTeams; i++)
		if (teamArr[i]->id == teamNum)
			return i;
	return -1;
}
int readGameFiles(team** teamArr, int gamesMat[][PLAYERS_NUM], int numOfTeams) {
	FILE* f;
	game g1;
	game g2;
	int goals;
	int id;
	int num_of_player;
	char txt[20];
	char txt1[20] = "./g";
	char txt2[20] = ".txt";
	char txt3[20];
	int txt_num = 1;
	f=fopen("./g1.txt","r");
	if (f == NULL)
		return -1;
	do {
		fscanf(f, "%d,%d,%d", &g1.id, &g1.goals, &g1.redcart);
		goals = g1.goals;
		id = getPos(teamArr, g1.id, numOfTeams);
		teamArr[id]->numOfGames++;
		teamArr[id]->goals += g1.goals;
		teamArr[id]->redCards += g1.redcart;
		while (goals-- > 0) {
			fscanf(f, ",%d", &num_of_player);
			gamesMat[id][num_of_player]++;
		}
		fscanf(f, "%d,%d,%d", &g2.id, &g2.goals, &g2.redcart);
		goals = g2.goals;
		id = getPos(teamArr, g2.id, numOfTeams);
		teamArr[id]->numOfGames++;
		teamArr[id]->goals += g2.goals;
		teamArr[id]->redCards += g2.redcart;
		while (goals-- > 0) {
			fscanf(f, ",%d", &num_of_player);
			gamesMat[id][num_of_player]++;
		}
		if (g1.goals > g2.goals) 
			teamArr[getPos(teamArr, g1.id, numOfTeams)]->score+=2;
		else if(g1.goals<g2.goals)
			teamArr[getPos(teamArr, g2.id, numOfTeams)]->score += 2;
		else {
			teamArr[getPos(teamArr, g1.id, numOfTeams)]->score++;
			teamArr[getPos(teamArr, g2.id, numOfTeams)]->score++;//equal one point
		}
		fclose(f);
		txt_num++;
		sprintf(txt3, "%d", txt_num);
		strcpy(txt, txt1);
		strcat(txt, txt3);
		strcat(txt, txt2);
		f = fopen(txt, "r");
	} while (f != NULL);
	num_of_games = txt_num-1;
	return num_of_games;
}
void print_spaces(int n) {
	while (n--)
		putchar(' ');
}
int count_dig(int n) {
	int count = 0;
	if (n == 0)
		return 1;
	while (n > 0) {
		n /= 10;
		count++;
	}
	return count;
}
void printSingleTeam(team* t) {
	printf("%d", t->id);
	print_spaces(8 - count_dig(t->id));
	printf("%d", t->score);
	print_spaces(8 - count_dig(t->score));
	printf("%d", t->redCards);
	print_spaces(8 - count_dig(t->redCards));
	printf("%d", t->numOfGames);
	print_spaces(8 - count_dig(t->numOfGames));
	printf("%d", t->goals);
	print_spaces(8 - count_dig(t->goals));
	puts(t->name);
}
void printTeamList(team** teamArr, int numOfTeams) {
	int i;
	//printf("The list of %d participants of teams\n", numOfTeams);
	printf("Num     Score   RedC    Games   Goals    Name\n");
	for (i = 0; i < numOfTeams; i++)
		printSingleTeam(teamArr[i]);
}
int get_max_goals() {
	int count = 0,i,j;
	for (i = 0; i < TEAM_NUM; i++)
		for(j=0;j<PLAYERS_NUM;j++)
			if (count < gamesMat[i][j])
				count = gamesMat[i][j];
	return count;
}

void printBestPlayers(team** teamArr, int gamesMat[][PLAYERS_NUM], int numOfTeams) {
	int goals = get_max_goals();
	printf("list of best players that scores %d goals\n", goals);
	for (int i = 0; i < TEAM_NUM; i++)
		for (int j = 0; j < PLAYERS_NUM; j++)
			if (gamesMat[i][j] == goals)
				printf("Team: %d-%s ,player number: %d\n", teamArr[i]->id, teamArr[i]->name, j);
}
void printTeamsByScore(team** teamArr, int numOfTeams) {
	//dont remember the name of sorting probably linear
	int i, j;
	team** t = get_copy_team_arr();
	team* temp;
	for(i=0;i<numOfTeams-1;i++)
		for(j=i+1;j<numOfTeams;j++)
			if (t[i]->score < t[j]->score) {
				temp = t[i];
				t[i] = t[j];
				t[j] = temp;
			}
	printf("The list of %d participants teams Sort by Score:\n",numOfTeams);
	printTeamList(t, numOfTeams);
	free_copy_of_team_arr(t);
}
void printTeamsByGames(team** teamArr, int numOfTeams) {
	//bubble sort
	int  i, j;
	team* temp;
	team** t = get_copy_team_arr();
	for(i=1;i<numOfTeams;i++)
		for(j=0;j<numOfTeams-i;j++)
			if (t[j]->numOfGames < t[j + 1]->numOfGames) {
				temp = t[j];
				t[j] = t[j + 1];
				t[j + 1] = temp;
			}
	printf("The list of %d participants teams Sort by Number Of Games::\n", numOfTeams);
	printTeamList(t, numOfTeams);
	free_copy_of_team_arr(t);
}
void printTeamsByGoals(team** teamArr, int numOfTeams) {
	//insertion sort
	int  i, j;
	team* temp,*key;
	team** t = get_copy_team_arr();
	for (i = 1; i < numOfTeams; i++) {
		j = i - 1;
		key = t[i];
		while (j >= 0 && t[j]->goals < key->goals) {
			t[j + 1] = t[j];
			j--;
		}
		t[j + 1] = key;
	}
	printf("The list of %d participants teams Sort by Number Of Goals::\n", numOfTeams);
	printTeamList(t, numOfTeams);
	free_copy_of_team_arr(t);
}
void printTeamsByRedCards(team** teamArr, int numOfTeams) {
	//insertion sort
	int  i, j;
	team* temp, * key;
	team** t = get_copy_team_arr();
	for (i = 1; i < numOfTeams; i++) {
		j = i - 1;
		key = t[i];
		while (j >= 0 && t[j]->redCards < key->redCards) {
			t[j + 1] = t[j];
			j--;
		}
		t[j + 1] = key;
	}
	printf("The list of %d participants teams Sort by Number Of Red Cards:\n", numOfTeams);
	printTeamList(t, numOfTeams);
	free_copy_of_team_arr(t);
}
game getTeamGame(team** teamArr, int numOfTeams, int teamID) {
	int i = 0,goals,p;
	game g;
	g.arr_of_players_goals[0] = -1;
	if (teamID == -1)
		puts("Get hostess team details.");
	else
		puts("Get guest team details.");
	printf("Insert team ID:");
	scanf("%d", &g.id);
	while (getPos(teamArr, g.id, numOfTeams) == -1 || teamID == g.id && teamID != -1) {
		if (teamID == g.id)
			puts("Same ID as hostess Team !!!, Try again");
		else
			puts("Wrong team ID !!!, Try again");
		scanf("%d", &g.id);
	}
	printf("Insert Goals number (0-9):");
	scanf("%d", &g.goals);
	while (g.goals > 9 || g.goals < 0) {
		puts("Wrong value insert number between 0 to 9");
		scanf("%d", &g.goals);
	}
	printf("Insert Red Cards number (0-9):");
	scanf("%d", &g.redcart);
	while (g.redcart > 9 || g.redcart < 0) {
		puts("Wrong value insert number between 0 to 9");
		scanf("%d", &g.redcart);
	}
	if (g.goals > 0) {
		printf("Insert %d number of players (1-25):\n",g.goals);
		goals = g.goals;
		while (goals > 0) {
			scanf("%d", &p);
			while (p > 25 || p < 1) {
				puts("Wrong number !!!,Try again");
				scanf("%d", &p);
			}
			goals--;
			g.arr_of_players_goals[i++] = p;
			g.arr_of_players_goals[i] = -1;
		}
	}
	return g;
}
void print_menu() {
	puts("Welcome to Mediterranean Football Games Managment System:");
	puts("1. Read Teams and Games Files..");
	puts("2. Insert a new game.");
	puts("3. Print Teams sorted by Score.");
	puts("4. Print Teams sorted by Red Cards.");
	puts("5. Print Teams sorted by Total Num of Goals.");
	puts("6. Print Teams sorted by Num of Games.");
	puts("7. Print Best Players.");
	puts("8. Print Teams list.");
	puts("9. Exit.\n");
}
void play() {
	int Q,is_first=0,i,count=1;
	char txt[20];
	char txt2[20] = "./g";
	char txt3[20] = ".txt";
	char txt4[20];
	FILE* f;
	game g1, g2;
	while (1) {
		print_menu();
		scanf("%d", &Q);
		if (Q > 1 && Q < 9 && is_first == 0)
			puts("You should read Teams and Games files first !!! (option 1)");
		else {
			switch (Q) {
			case 1:
				if (readTeamFile(teamArr) == -1) {
					printf("Error reading team file exit\n");
					exit(1);
				}
				else
					printf("Reading teams file....\nTeams file succecfuly read\nnumOfTeams=%d\n", num_of_teams);
				if (readGameFiles(teamArr, gamesMat, num_of_teams) == -1) {
					printf("Error readint games files exit\n");
					exit(1);
				}
				else
					printf("Reading games files....\nGames files successfuly read\nnumOfGames=%d\n", num_of_games);
				is_first = 1;
				break;
			case 2:
				g1 = getTeamGame(teamArr, num_of_teams, -1);
				g2 = getTeamGame(teamArr, num_of_teams, g1.id);
				strcpy(txt, txt2);
				txt4[0] = 0;
				sprintf(txt4, "%d", ++num_of_games);
				strcat(txt, txt4);
				strcat(txt, txt3);
				f = fopen(txt, "w");
				fprintf(f, "%d,%d,%d", g1.id, g1.goals, g1.redcart);
				i = 0;
				while (g1.arr_of_players_goals[i++] != -1)
					fprintf(f, ",%d", g1.arr_of_players_goals[i - 1]);
				fprintf(f, "\n");
				fprintf(f, "%d,%d,%d", g2.id, g2.goals, g2.redcart);
				i = 0;
				while (g2.arr_of_players_goals[i++] != -1)
					fprintf(f, ",%d", g2.arr_of_players_goals[i - 1]);
				if (g1.goals > g2.goals)
					teamArr[getPos(teamArr, g1.id, num_of_teams)]->score += 2;
				else if (g1.goals < g2.goals)
					teamArr[getPos(teamArr, g2.id, num_of_teams)]->score += 2;
				else {
					teamArr[getPos(teamArr, g1.id, num_of_teams)]->score++;
					teamArr[getPos(teamArr, g2.id, num_of_teams)]->score++;//equal one point
				}
				break;
			case 3:
				printTeamsByScore(teamArr, num_of_teams);
				break;
			case 4:
				printTeamsByRedCards(teamArr, num_of_teams);
				break;
			case 5:
				printTeamsByGoals(teamArr, num_of_teams);
				break;
			case 6:
				printTeamsByGames(teamArr, num_of_teams);
				break;
			case 7:
				printBestPlayers(teamArr, gamesMat, num_of_teams);
				break;
			case 8:
				printf("The list of %d participants of teams\n", num_of_teams);
				printTeamList(teamArr, num_of_teams);
				break;
			case 9:
				exit(1);
				break;
			}
		}
		while(count<5&&(Q > 9 || Q < 1)) {
			puts("Wrong input!!! try again...");
			scanf("%d", &Q);
			count++;
		}
		if (count == 5) {
			printf("You have made 5 menu errors, bye bye!!\n");
			break;
		}
	}
}
int main() {
	play();
	return 0;
}