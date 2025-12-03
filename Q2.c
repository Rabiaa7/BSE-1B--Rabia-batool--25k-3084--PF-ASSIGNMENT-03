//25k-3084 (space fuel mission)
#include <stdio.h>

int calculateFuel(int fuel, int consumption, int recharge, int solarBonus,
                  int planet, int totalPlanets)
{
    if (fuel <= 0)
    {
        printf("Planet %d: Fuel Remaining = 0 (Fuel Exhausted) \n Mission failed!", planet);
        return 0;
    }
    if (planet > totalPlanets)
    {
        printf("Mission succesful!");
        return fuel;
    }
    fuel -= consumption;
    fuel += recharge;

    if (planet % 4 == 0)
    {
        fuel += solarBonus;
    }
    if (fuel < 0)
        fuel = 0;

    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    return calculateFuel(fuel, consumption, recharge, solarBonus,
                         planet + 1, totalPlanets);
}

int main()
{
    int fuel = 500;        
    int consumption = 50;    
    int recharge = 15;      
    int solarBonus = 30;    
    int totalPlanets = 7;   

    printf("Mission space:\n");

    int finalFuel = calculateFuel(fuel, consumption, recharge,
                                  solarBonus, 1, totalPlanets);

    if (finalFuel > 0)
        printf("\nMission Completed. Final Fuel Remaining: %d\n", finalFuel);
    else
        printf("\nMission Failed. Fuel Exhausted.\n");

    return 0;
}
