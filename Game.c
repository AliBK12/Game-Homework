#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 10    //Sabit değerleri atama
#define MAX_ITEMS 5
#define MAX_INVENTORY 5
#define MAX_CMD_LEN 50


typedef struct Item {   //Öğelerin yapıları
    char name[50];
} Item;

typedef struct Enemy {
    char name[50];
    int health;
} Enemy;

typedef struct Room {
    char description[200];
    struct Room *up, *down, *left, *right;
    Item *items[MAX_ITEMS];
    Enemy *Enemy;
} Room;



typedef struct Player {
    int health, strength, armor, inventory_count;
    Item *inventory[MAX_INVENTORY];
    Room *current_room;
} Player;

void start(Player *player, Room *rooms);  //Kullanılan metodlar
void keywords(char *cmd, Player *player);
void player_move(char *dir, Player *player);
void look(Player *player);
void pickup_item(Player *player, char *item_name);
void inventory(Player *player);
void attack(Player *player);
void show_health(Player *player);
void save_game(Player *player, const char *filename);
void load_game(Player *player, const char *filename);
Room *create_room(const char *desc);
Item *create_item(const char *name);
Enemy *create_Enemy(const char *name, int health);
void link_rooms(Room *room1, Room *room2, const char *dir);
void fight(Player *player, Enemy *Enemy);

int main() {    //Main metodu
    Player player;
    Room rooms[MAX_ROOMS];
    char cmd[MAX_CMD_LEN];

    start(&player, rooms);

    printf("Welcome to Dungeon Adventure!!!\n");
    while (1) {
        printf("\n> ");
        fgets(cmd, MAX_CMD_LEN, stdin);
        cmd[strcspn(cmd, "\n")] = '\0';
        if (strcmp(cmd, "exit") == 0) 
        keywords(cmd, &player);
        break; 
    }

    return 0;
}

void start(Player *player, Room *rooms) {     //Başlangıç metodu ve gerekli atamalar
    rooms[0] = *create_room("You are in the spawn room. This place is spooky...");
    rooms[1] = *create_room("You moved to the upper floor. There are claw marks everywhere in here.");
    rooms[2] = *create_room("You moved to the basement. You see rotten rats and skeletons.");
    rooms[3] = *create_room("You moved to the left room. Cobwebs and dust fills this room.");
    rooms[4] = *create_room("You moved to the right room. There is nothing here. Just walls");

    link_rooms(&rooms[0], &rooms[1], "up");
    link_rooms(&rooms[0], &rooms[2], "down");
    link_rooms(&rooms[0], &rooms[4], "right");
    link_rooms(&rooms[0], &rooms[3], "left");
    

    Item *sword = create_item("Sword");
    rooms[3].items[0] = sword;
    Item *armor = create_item("Armor");
    rooms[1].items[0] = armor;
    Item *torch = create_item("Torch");
    rooms[2].items[0] = torch;

    Enemy *werewolf = create_Enemy("Werewolf", 20);
    rooms[1].Enemy = werewolf;
    Enemy *spider = create_Enemy("Huge Spider", 25);
    rooms[3].Enemy = spider;
    Enemy *zombie = create_Enemy("Zombie", 20);
    rooms[2].Enemy = zombie;

    player->health = 100;
    player->strength = 5;
    player->armor = 0;
    player->inventory_count = 0;
    player->current_room = &rooms[0];
}

void keywords(char *cmd, Player *player) {    //Oyunu oynamak için gerekli keywordler
    if (strncmp(cmd, "move", 4) == 0) player_move(cmd + 5, player);
    else if (strcmp(cmd, "look") == 0) look(player);
    else if (strncmp(cmd, "pickup", 6) == 0) pickup_item(player, cmd + 7);
    else if (strcmp(cmd, "inventory") == 0) inventory(player);
    else if (strcmp(cmd, "attack") == 0) attack(player);
    else if (strcmp(cmd, "health") == 0) show_health(player);
    else if (strncmp(cmd, "save", 4) == 0) save_game(player, cmd + 5);
    else if (strncmp(cmd, "load", 4) == 0) load_game(player, cmd + 5);
    else printf("Invalid command.\n");
}

void player_move(char *dir, Player *player) {  //karakter hareketleri
    Room *next = NULL;
    if (strcmp(dir, "up") == 0) next = player->current_room->up;
    else if (strcmp(dir, "down") == 0) next = player->current_room->down;
    else if (strcmp(dir, "left") == 0) next = player->current_room->left;
    else if (strcmp(dir, "right") == 0) next = player->current_room->right;

    if (next) {
        player->current_room = next;
        look(player);
    } else {
        printf("Can't move to that direction.\n");
    }
}

void look(Player *player) {         //Odayı incelemek için look metodu
    printf("%s\n", player->current_room->description);
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->current_room->items[i]) 
            printf("You see a %s here.\n", player->current_room->items[i]->name);
    }
    if (player->current_room->Enemy)
        printf("A %s is here with %d health.\n", player->current_room->Enemy->name, player->current_room->Enemy->health);
}

void pickup_item(Player *player, char *item_name) {      //Eşya toplama metodu
    if (player->inventory_count >= MAX_INVENTORY) {
        printf("Your inventory is full!\n");
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (player->current_room->items[i] && strcmp(player->current_room->items[i]->name, item_name) == 0) {
            player->inventory[player->inventory_count++] = player->current_room->items[i];
            player->current_room->items[i] = NULL;
            if (strcmp(item_name, "Armor") == 0) player->armor = 5;
            printf("Picked up %s.\n", item_name);
            return;
        }
    }

    printf("No %s here.\n", item_name);
}

void inventory(Player *player) {   //Envanteri gösterme metodu
    printf("Your inventory:\n");
    for (int i = 0; i < player->inventory_count; i++) {
        printf("- %s\n", player->inventory[i]->name);
    }
}

void attack(Player *player) {  //Saldırı metodu
    Enemy *Enemy = player->current_room->Enemy;
    if (!Enemy) {
        printf("There is nothing to fight!!!\n");
        return;
    }
    printf("You attack the %s!!!\n", Enemy->name);
    fight(player, Enemy);
}

void fight(Player *player, Enemy *Enemy) {    // Temel dövüş mekaniklerini içeren savaş metodu. Turn based sistem
    while (player->health > 0 && Enemy->health > 0) {
        int damage_taken = player->strength - player->armor;
        if (damage_taken < 0) damage_taken = 0;
        Enemy->health -= damage_taken;

        if (Enemy->health < 0) Enemy->health = 0;

        printf("You hit %s! It takes %d damage. Health: %d\n", Enemy->name, damage_taken, Enemy->health);

        if (Enemy->health == 0) {
            printf("You defeated the %s!\n", Enemy->name);
            player->current_room->Enemy = NULL;
            return;
        }

        int damage_player_took = Enemy->health / 5;
        player->health -= damage_player_took;
        printf("The %s attacks you! You take %d damage. Health: %d\n", Enemy->name, damage_player_took, player->health);

        if (player->health <= 0) {
            printf("You were defeated by the %s.\n", Enemy->name);
            return;
        }
    }
}


void show_health(Player *player) {    // Sağlık gösterme metodu
    printf("Health: %d\n", player->health);
    if (player->armor > 0) printf("Armor: %d\n", player->armor);
}

Room *create_room(const char *desc) {  //Odaların yaratılışı dinamik olarak
    Room *room = malloc(sizeof(Room));
    strcpy(room->description, desc);
    room->up = room->down = room->left = room->right = NULL;
    memset(room->items, 0, sizeof(room->items));
    room->Enemy = NULL;
    return room;
}

Item *create_item(const char *name) {   //Dinamik olarak itemlerin yaratılışı
    Item *item = malloc(sizeof(Item));
     strcpy(item->name, name);
    return item;
}

Enemy *create_Enemy(const char *name, int health) {  //Dinamik olarak düşmanların yaratılışı
    Enemy *Enemy = malloc(sizeof(Enemy));
    strcpy(Enemy->name, name);
    Enemy->health = health;
    return Enemy;
}

void link_rooms(Room *room1, Room *room2, const char *dir) {  //Odaları birbirine bağlama. + sembolü şeklinde bir oda yapısı. Ortada bir oda ve her yanında 1'er oda olacak şekilde
    if (strcmp(dir, "up") == 0) {
        room1->up = room2; room2->down = room1;
    } else if (strcmp(dir, "down") == 0) {
        room1->down = room2; room2->up = room1;
    } else if (strcmp(dir, "left") == 0) {
        room1->left = room2; room2->right = room1;
    } else if (strcmp(dir, "right") == 0) {
        room1->right = room2; room2->left = room1;
    }
}

void load_game(Player *player, const char *filename) {  //Oyunu yükleme
    FILE *file = fopen(filename, "l");
    if (!file) { printf("Failed to load game.\n"); return; }
    fscanf(file, "%d %d %d %d\n", &player->health, &player->strength, &player->armor, &player->inventory_count);
    for (int i = 0; i < player->inventory_count; i++) {
        char item_name[50];
        fscanf(file, "%s\n", item_name);
        player->inventory[i] = create_item(item_name);
    }
    fclose(file);
    printf("Game loaded.\n");
}

void save_game(Player *player, const char *filename) {    //Oyunu kaydetme
    FILE *file = fopen(filename, "s");
    if (!file) { printf("Failed to save \n"); return; }
    fprintf(file, "%d %d %d %d\n", player->health, player->strength, player->armor, player->inventory_count);
    for (int i = 0; i < player->inventory_count; i++) {
        fprintf(file, "%s\n", player->inventory[i]->name);
    }
    fclose(file);
    printf("Game saved.\n");
}