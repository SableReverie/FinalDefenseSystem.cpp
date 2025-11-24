#include <iostream> // For console outpout and input
#include <vector> // For using vectors
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time() 
#include <limits> // For numeric_limits = input safety 
#include <windows.h> // For console color and sound

using namespace std;

// ==================== CONSTANTS ====================
const int NODE_COUNT = 3;
const int STARTING_ENERGY = 10;
const int STARTING_HP = 20;
const int ENERGY_PER_ROUND = 3;
const int MAX_ROUNDS = 20;

// ==================== STRUCT ====================
struct Quiz {
    string question;
    int answer;
    vector<int> options; // possible choices
};

struct SkillCooldown 
{
    int doubleStrike;
    int shield;
    int powerSurge;
    int emergencyRepair;
    
    SkillCooldown() : doubleStrike(0), shield(0), powerSurge(0), emergencyRepair(0) {}
    
    void decreaseCooldowns() 
    {
        if (doubleStrike > 0) 
        {
            doubleStrike--;
        }
        if (shield > 0) 
        {
            shield--;
        }
        if (powerSurge > 0) 
        {
            powerSurge--;
        }
        if (emergencyRepair > 0) 
        {
            emergencyRepair--;
        }
    }
    
    bool isOnCooldown(int skillNum) const 
    {
        switch (skillNum) 
        {
            case 1: 
                return doubleStrike > 0;
            case 2: 
                return shield > 0;
            case 3: 
                return powerSurge > 0;
            case 4: 
                return emergencyRepair > 0;
            default: 
                return false;
        }
    }
    
    int getCooldownRemaining(int skillNum) const 
    {
        switch (skillNum) 
        {
            case 1: 
                return doubleStrike;
            case 2: 
                return shield;
            case 3: 
                return powerSurge;
            case 4: 
                return emergencyRepair;
            default: 
                return 0;
        }
    }
    
    void setCooldown(int skillNum, int rounds) 
    {
        switch (skillNum) 
        {
            case 1: 
                doubleStrike = rounds; 
                break;
            case 2: 
                shield = rounds; 
                break;
            case 3: 
                powerSurge = rounds; 
                break;
            case 4: 
                emergencyRepair = rounds; 
                break;
        }
    }
};

struct GameState 
{
    vector<int> playerNodes;
    SkillCooldown skillCooldowns;
    int playerEnergy;
    int playerHP;
    int cpuHP;
    int roundNum;
    int currentBoost;
    bool playerQuit;
    
    GameState() : playerNodes(NODE_COUNT, 0),
                 playerEnergy(STARTING_ENERGY), playerHP(STARTING_HP), 
                 cpuHP(STARTING_HP), roundNum(1), currentBoost(0) {}
    
    void reset() 
    {
        playerNodes = vector<int>(NODE_COUNT, 0);
        skillCooldowns = SkillCooldown();
        playerEnergy = STARTING_ENERGY;
        playerHP = STARTING_HP;
        cpuHP = STARTING_HP;
        roundNum = 1;
        currentBoost = 0;
    }
};

// ==================== COLOR FUNCTIONS ====================
void setColor(int color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void printColored(const string& text, int color) 
{
    setColor(color);
    cout << text;
    resetColor();
}

// ==================== SOUND FUNCTIONS ====================
void soundSuccess() 
{ 
    Beep(1200, 120); 
}

void soundError() 
{ 
    Beep(300, 250); 
}

void soundMove() 
{ 
    Beep(800, 80); 
}

void soundAttack() 
{ 
    Beep(600, 150); 
}

void soundVictory() 
{ 
    Beep(1000, 300); 
    Beep(1200, 300); 
    Beep(1500, 500); 
}

// ==================== INPUT SAFETY ====================
void clearInput() 
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==================== QUIZ DATA ====================
vector<Quiz> createQuizzes() 
{
    return {
        {"1 AND 0 =", 0, {0, 1}},
        {"1 AND 1 =", 1, {0, 1}},
        {"0 OR 1 =", 1, {0, 1}},
        {"NOT 0 =", 1, {0, 1}}
    };
}

vector<Quiz> createSkillQuizzes() 
{
    return {
        {"1 OR 0 =", 1, {0, 1}},
        {"0 OR 0 =", 0, {0, 1}},
        {"NOT 1 =", 0, {0, 1}},
        {"1 AND 0 =", 0, {0, 1}}
    };
}

// ==================== DISPLAY FUNCTIONS ====================
void printHealth(const string& name, int hp, int maxHp) 
{
    cout << name << " HP: [";
    
    const int barLength = 20;
    int filled = (hp * barLength) / maxHp;
    
    // Choose color based on health percentage
    int color = (hp > maxHp * 0.6) ? 10 : (hp > maxHp * 0.3) ? 14 : 12;
    
    setColor(color);
    for (int i = 0; i < filled; i++) 
    {
        cout << "=";
    }
    resetColor();
    
    for (int i = filled; i < barLength; i++) 
    {
        cout << " ";
    }
    
    cout << "] ";
    setColor(color);
    cout << hp << "/" << maxHp;
    resetColor();
    cout << "\n";
}

void printRoundInfo(const GameState& state) 
{
    cout << "\n--- Round " << state.roundNum << "/" << MAX_ROUNDS << " ---\n";
    cout << "Rounds remaining: " << (MAX_ROUNDS - state.roundNum + 1) << "\n"; 
}

void printBoard(const GameState& state) 
{
    cout << "\nYour Nodes (Energy: ";
    printColored(to_string(state.playerEnergy), 11);
    cout << " | Boost: ";
    printColored(to_string(state.currentBoost), 14);
    cout << ")\n";
    
    for (int i = 0; i < state.playerNodes.size(); i++) 
    {
        cout << "Node " << i << ": ";
        
        if (state.playerNodes[i] == 1) 
        {
            printColored("ON", 10);
            cout << " (attack power: " << ((i + 1) * 2) << ")";
        } 
        else 
        {
            printColored("OFF", 8);
            cout << " (+" << (i + 1) << " defense)";
        }
        
        
        cout << "\n";
    }
}

void printSkillCooldowns(const SkillCooldown& cooldowns) 
{
    cout << "\n";
    printColored("🛡️  SKILL COOLDOWNS:\n", 13);
    
    auto printSkillStatus = [&](int skillNum, const string& name) 
    {
        cout << "• " << name << ": ";
        if (cooldowns.isOnCooldown(skillNum)) 
        {
            printColored("Cooldown " + to_string(cooldowns.getCooldownRemaining(skillNum)) + " rounds", 14);
        } 
        else 
        {
            printColored("Ready", 10);
        }
        cout << "\n";
    };
    
    printSkillStatus(1, "Double Strike");
    printSkillStatus(2, "Shield");
    printSkillStatus(3, "Power Surge");
    printSkillStatus(4, "Emergency Repair");
}

// ==================== GAME MECHANICS ====================
int getNodeCost(int nodeIndex) 
{
    return (nodeIndex + 1) * 2;
}

int calculateDefenseBonus(const vector<int>& nodes) 
{
    int defense = 0;
    for (int i = 0; i < nodes.size(); i++) 
    {
        if (nodes[i] == 0) 
        {
            defense += (i + 1);
        }
    }
    return defense;
}

int calculateAttackPower(const vector<int>& nodes, int boost) 
{
    int attack = boost; // Start with boost
    
    for (int i = 0; i < nodes.size(); i++) 
    {
        if (nodes[i] == 1) 
        {
            attack += (i + 1) * 2; // Node 0: 2 damage, Node 1: 4 damage, Node 2: 6 damage
        }
    }
    
    // Random critical hit chance (20%)
    if (rand() % 5 == 0 && attack > 0) 
    {
        attack = attack * 1.5;
        printColored("Critical hit! ", 12);
    }
    
    return attack;
}

int calculateCPUDefense() 
{
    int defense = rand() % 4 + 2; // Base defense 2-5
    
    // Random chance for extra defense
    if (rand() % 4 == 0) 
    {
        int extraDefense = rand() % 3 + 1;
        defense += extraDefense;
        cout << "CPU activates shield! +" << extraDefense << " defense.\n";
    }
    
    return defense;
}

int calculateCPUAttack() 
{
    int attack = rand() % 4 + 3; // 3-6 damage
    
    // Random chance for powerful attack
    if (rand() % 5 == 0) 
    {
        attack += 3;
        printColored("CPU uses powerful attack! ", 12);
    }
    
    return attack;
}

// ==================== QUIZ FUNCTIONS ====================
bool askQuiz(const string& prompt, const vector<Quiz>& quizzes) 
{
    int idx = rand() % quizzes.size();
    const Quiz& currentQuiz = quizzes[idx];
    
    cout << "\n💡 ";
    printColored(prompt, 14);
    cout << ": " << currentQuiz.question << " ";
    
    // Display options
    cout << "(Options: ";
    for (size_t i = 0; i < currentQuiz.options.size(); i++) 
    {
        cout << currentQuiz.options[i];
        if (i < currentQuiz.options.size() - 1) 
        {
            cout << "/";
        }
    }
    cout << ") ";
    
    int answer;
    if (!(cin >> answer)) 
    {
        clearInput();
        soundError();
        return false;
    }
    
    bool correct = (answer == currentQuiz.answer);
    
    if (correct) 
    {
        printColored("Correct!\n", 10);
        soundSuccess();
    } 
    else 
    {
        printColored("Wrong. The answer was " + to_string(currentQuiz.answer) + ".\n", 12);
        soundError();
    }
    
    return correct;
}

void executeBattle(GameState& state) 
{
    auto quizzes = createQuizzes();
    
    cout << "\n🎯 ";
    printColored("BATTLE QUIZ", 14);
    cout << " (correct = you attack, wrong = CPU attacks)\n";
    
    if (askQuiz("Answer correctly to attack", quizzes)) 
    {
        // Player attacks
        int attack = calculateAttackPower(state.playerNodes, state.currentBoost);
        int defense = calculateCPUDefense();
        int damage = max(0, attack - defense);

        cout << "\n⚔️  YOUR ATTACK:\n";
        cout << "Attack power: " << attack << " (base: " << (attack - state.currentBoost) << " + boost: " << state.currentBoost << ")\n";
        cout << "CPU defense: " << defense << "\n";
        
        if (damage > 0) 
        {
            cout << "CPU takes " << damage << " damage!\n";
            state.cpuHP -= damage;
            if (state.cpuHP < 0) 
            {
                state.cpuHP = 0;
            }
            soundAttack();
        } 
        else 
        {
            cout << "CPU blocks all damage!\n";
        }
    } 
    else 
    {
        // CPU attacks
        int cpuAttack = calculateCPUAttack();
        int playerDefense = calculateDefenseBonus(state.playerNodes);
        int damage = max(1, cpuAttack - playerDefense);
        
        cout << "\n🤖 CPU ATTACK:\n";
        cout << "CPU attacks with " << cpuAttack << " power\n";
        if (playerDefense > 0) 
        {
            cout << "Your defense: " << playerDefense << "\n";
        }
        cout << "You take " << damage << " damage!\n";
        
        state.playerHP -= damage;
        if (state.playerHP < 0) 
        {
            state.playerHP = 0;
        }
        
        soundError();
    }
}

// ==================== SKILL SYSTEM ====================
void useSkill(GameState& state) 
{
    cout << "\n";
    printColored("🛡️ AVAILABLE SKILLS:\n", 13);
    
    auto printSkill = [&](int num, const string& name, int cost, int cooldown, bool onCooldown, int roundsLeft) 
    {
        cout << num << ". " << name << " (" << cost << " energy)";
        if (onCooldown) 
        {
            printColored(" [Cooldown: " + to_string(roundsLeft) + " rounds]", 14);
        } 
        else 
        {
            printColored(" [Ready]", 10);
        }
        cout << "\n";
    };
    
    printSkill(1, "Double Strike - +2 attack boost", 3, 3, 
               state.skillCooldowns.isOnCooldown(1), state.skillCooldowns.getCooldownRemaining(1));
    printSkill(2, "Shield - Restore 2 HP", 2, 4, 
               state.skillCooldowns.isOnCooldown(2), state.skillCooldowns.getCooldownRemaining(2));
    printSkill(3, "Power Surge - +2 attack boost", 4, 2, 
               state.skillCooldowns.isOnCooldown(3), state.skillCooldowns.getCooldownRemaining(3));
    printSkill(4, "Emergency Repair - +3 defense next round", 3, 5, 
               state.skillCooldowns.isOnCooldown(4), state.skillCooldowns.getCooldownRemaining(4));
    
    cout << "Enter skill number or 0 to cancel: ";
    
    int skillNum;
    cin >> skillNum;
    
    if (cin.fail()) 
    {
        clearInput();
        soundError();
        cout << "Invalid input.\n";
        return;
    }
    
    // Check if skill is on cooldown first
    if (skillNum >= 1 && skillNum <= 4 && state.skillCooldowns.isOnCooldown(skillNum)) 
    {
        soundError();
        printColored("Skill is on cooldown for " + to_string(state.skillCooldowns.getCooldownRemaining(skillNum)) + " more rounds!\n", 14);
        return;
    }
    
    auto skillQuizzes = createSkillQuizzes();
    bool success = false;
    
    switch (skillNum) 
    {
        case 0:
            cout << "Skill cancelled.\n";
            return;
            
        case 1:
            if (state.playerEnergy >= 3) 
            {
                state.playerEnergy -= 3;
                success = askQuiz("Skill Quiz: Double Strike", skillQuizzes);
                if (success) 
                {
                    state.currentBoost += 2;
                    state.skillCooldowns.setCooldown(1, 3); // 3-round cooldown
                    printColored("Double Strike activated! +2 attack boost. (Cooldown: 3 rounds)\n", 10);
                }
            } 
            else 
            {
                soundError();
                cout << "Not enough energy.\n";
            }
            break;
            
        case 2:
            if (state.playerEnergy >= 2) 
            {
                state.playerEnergy -= 2;
                success = askQuiz("Skill Quiz: Shield", skillQuizzes);
                if (success) 
                {
                    state.playerHP = min(STARTING_HP, state.playerHP + 2);
                    state.skillCooldowns.setCooldown(2, 4); // 4-round cooldown
                    printColored("Shield restored 2 HP! (Cooldown: 4 rounds)\n", 10);
                    printHealth("Player", state.playerHP, STARTING_HP);
                }
            } 
            else 
            {
                soundError();
                cout << "Not enough energy.\n";
            }
            break;
            
        case 3:
            if (state.playerEnergy >= 4) 
            {
                state.playerEnergy -= 4;
                success = askQuiz("Skill Quiz: Power Surge", skillQuizzes);
                if (success) 
                {
                    state.currentBoost += 2;
                    state.skillCooldowns.setCooldown(3, 2); // 2-round cooldown
                    printColored("Power Surge activated! +2 attack boost. (Cooldown: 2 rounds)\n", 10);
                }
            } 
            else 
            {
                soundError();
                cout << "Not enough energy.\n";
            }
            break;
            
        case 4:
            if (state.playerEnergy >= 3) 
            {
                state.playerEnergy -= 3;
                success = askQuiz("Skill Quiz: Emergency Repair", skillQuizzes);
                if (success) 
                {
                    // Temporary defense boost
                    state.currentBoost += 1;
                    state.skillCooldowns.setCooldown(4, 5); // 5-round cooldown
                    printColored("Emergency Repair! +3 temporary defense. (Cooldown: 5 rounds)\n", 10);
                }
            } 
            else 
            {
                soundError();
                cout << "Not enough energy.\n";
            }
            break;
            
        default:
            soundError();
            cout << "Invalid skill number.\n";
            break;
    }
}

// ==================== COMMAND PROCESSING ====================
bool processCommand(GameState& state, const string& command) 
{
    if (command == "quit") 
    {
        cout << "\nYou quit the game.\n";
        return false;
    }
    
    if (command == "energy") 
    {
        cout << "Energy: ";
        printColored(to_string(state.playerEnergy) + "/" + to_string(STARTING_ENERGY) + "\n", 11);
        return true;
    }
    
    if (command == "show") 
    {
        printBoard(state);
        printSkillCooldowns(state.skillCooldowns);
        return true;
    }
    
    if (command == "cooldowns") 
    {
        printSkillCooldowns(state.skillCooldowns);
        return true;
    }
    
    if (command == "set") 
    {
        int node, val;
        cin >> node >> val;
        
        if (cin.fail() || node < 0 || node >= NODE_COUNT) 
        {
            clearInput();
            soundError();
            cout << "Usage: set <0-" << NODE_COUNT-1 << "> <0/1>\n";
            return true;
        }
        
        
        
        val = (val == 1) ? 1 : 0;
        
        if (val == 1 && state.playerNodes[node] == 0) 
        {
            int cost = getNodeCost(node);
            if (state.playerEnergy >= cost) 
            {
                state.playerEnergy -= cost;
                state.playerNodes[node] = 1;
                soundMove();
                cout << "Energy used: " << cost << ". Remaining: " << state.playerEnergy << "\n";
            } 
            else 
            {
                soundError();
                printColored("Not enough energy! Need " + to_string(cost) + " but have " + 
                           to_string(state.playerEnergy) + "\n", 12);
                return true;
            }
        } 
        else if (val == 0) 
        {
            state.playerNodes[node] = 0;
            soundMove();
            cout << "Node " << node << " set to OFF (+" << (node + 1) << " defense)\n";
        }
        
        printBoard(state);
        return true;
    }
    
    if (command == "skill") 
    {
        useSkill(state);
        return true;
    }
    
    if (command == "attack") 
    {
        executeBattle(state);
        
        // Advance round (nodes have no cooldown)
        
        // Decrease skill cooldowns
        state.skillCooldowns.decreaseCooldowns();
        
        state.roundNum++;
        return false; // Round finished
    }
    
    clearInput();
    soundError();
    cout << "Unknown command. Type 'set', 'attack', 'skill', 'show', 'cooldowns', 'energy', or 'quit'\n";
    return true;
}

// ==================== GAME INITIALIZATION ====================
void showInstructions() 
{
    cout << "=== LOGIC QUIZ BATTLE ===\n\n";
    
    printColored("🎯 HOW TO PLAY:\n", 11);
    cout << "• Battle lasts " << MAX_ROUNDS << " rounds maximum\n";
    cout << "• After " << MAX_ROUNDS << " rounds, player with most HP wins\n";
    cout << "• Use SET command to turn nodes ON/OFF\n";
    cout << "• ON nodes = Attack power | OFF nodes = Defense bonus\n";
    cout << "• Answer quizzes correctly to attack\n";
    cout << "• Wrong answers = CPU attacks you\n";
    cout << "• Skills have cooldowns - use them strategically!\n\n";
    
    printColored("⚡ ENERGY & COOLDOWNS:\n", 11);
    cout << "• Start with 10 energy, +3 each round\n";
    cout << "• Double Strike: 3 energy, 3-round cooldown\n";
    cout << "• Shield: 2 energy, 4-round cooldown\n";
    cout << "• Power Surge: 4 energy, 2-round cooldown\n";
    cout << "• Emergency Repair: 3 energy, 5-round cooldown\n\n";
    
    printColored("🎮 AVAILABLE COMMANDS:\n", 11);
    cout << "• set <node> <0/1>  - Turn node OFF/ON\n";
    cout << "• attack           - Start battle quiz\n";
    cout << "• skill            - Use special skills\n";
    cout << "• show             - Display nodes and skill cooldowns\n";
    cout << "• cooldowns        - Check skill cooldowns only\n";
    cout << "• energy           - Check energy\n";
    cout << "• quit             - Exit game\n\n";
    
    printColored("Ready to begin! Good luck!\n", 10);
    cout << "========================================\n";
}

// ==================== GAME OVER LOGIC ====================
bool determineWinner(const GameState& state) 
{
    cout << "\n=== GAME OVER ===\n";
    printHealth("Player", state.playerHP, STARTING_HP);
    printHealth("CPU", state.cpuHP, STARTING_HP);
    cout << "\n";
    
    if (state.playerHP <= 0) 
    {
        printColored("💀 CPU wins! You were defeated in battle.\n", 12);
    } 
    else if (state.cpuHP <= 0) 
    {
        printColored("🎉 You won the Logic Quiz Battle by defeating the CPU!\n", 10);
        soundVictory();
    } 
    else if (state.roundNum > MAX_ROUNDS) 
    {
        cout << "🏁 " << MAX_ROUNDS << " rounds completed!\n";
        if (state.playerHP > state.cpuHP) 
        {
            printColored("🎉 You win by having more HP! (" + to_string(state.playerHP) + " vs " + to_string(state.cpuHP) + ")\n", 10);
            soundVictory();
        } 
        else if (state.cpuHP > state.playerHP) 
        {
            printColored("💀 CPU wins by having more HP! (" + to_string(state.cpuHP) + " vs " + to_string(state.playerHP) + ")\n", 12);
        } 
        else 
        {
            printColored("🤝 It's a tie! Both have " + to_string(state.playerHP) + " HP.\n", 14);
        }
    }
    
    // Ask if player wants to play again
    cout << "\n";
    printColored("Would you like to play again? (y/n): ", 11);
    
    string playAgain;
    cin >> playAgain;
    clearInput();
    
    return (playAgain == "y" || playAgain == "Y" || playAgain == "yes" || playAgain == "YES");
}

// ==================== MAIN GAME LOOP ====================
GameState playGame() 
{
    GameState state;
    
    showInstructions();
    
    while (state.playerHP > 0 && state.cpuHP > 0 && state.roundNum <= MAX_ROUNDS) 
    {
        // Start of round setup
        state.playerEnergy = min(STARTING_ENERGY, state.playerEnergy + ENERGY_PER_ROUND);
        state.currentBoost = 0;
        
        
        // Reset nodes to OFF but keep cooldown status
        for (int i = 0; i < NODE_COUNT; i++) 
        {
            state.playerNodes[i] = 0;
        }
        
        printRoundInfo(state);
        printHealth("Player", state.playerHP, STARTING_HP);
        printHealth("CPU", state.cpuHP, STARTING_HP);
        printBoard(state);
        printSkillCooldowns(state.skillCooldowns);
        
        cout << "\nCommands: ";
        printColored("set <node> <0/1> | attack | skill | show | cooldowns | energy | quit\n", 15);
        
        // Command processing loop for current round
        bool roundFinished = false;
        bool shouldQuit = false;
        
        while (!roundFinished && state.playerHP > 0 && state.cpuHP > 0 && !shouldQuit) 
        {
            cout << "> ";
            string command;
            cin >> command;
            
            bool continueProcessing = processCommand(state, command);
            
            if (!continueProcessing) 
            {
                if (command == "quit") 
                {
                    shouldQuit = true;
                    break;
                } 
                else 
                {
                    roundFinished = true;
                }
            }
        }
        
        // If player quit, break out of the main game loop
        if (shouldQuit) 
        {
            state.playerQuit = true;
            break;
        }
        
        // Note: node cooldowns are managed as integer rounds and decremented at round start.
        
        // Check if we've reached the round limit
        if (state.roundNum > MAX_ROUNDS) 
        {
            break;
        }
    }
    return state;
}

// ==================== MAIN FUNCTION ====================
int main() 
{
    srand(time(0));
    bool playAgain = true;
    
    while (playAgain) 
    {
        GameState finalState = playGame();

        if (finalState.playerQuit) 
        {
            // Player quit to main menu — do not show winner, just stop
            playAgain = false;
            break;
        }

        playAgain = determineWinner(finalState);
        
        if (playAgain) 
        {
            cout << "\n";
            printColored("Starting a new game...\n", 10);
            cout << "========================================\n";
        }
    }
    
    cout << "\n";
    printColored("Thanks for playing Logic Quiz Battle!\n", 11);
    return 0;
}
