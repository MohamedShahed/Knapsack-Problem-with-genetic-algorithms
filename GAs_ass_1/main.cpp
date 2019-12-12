#include<iostream>
#include<cmath>
#include<time.h>
#include<algorithm>

using namespace std ;
int NumberOfPopulation=300;
int **Population=NULL;
int *FitnessArray=NULL;




void generatePopuation(int NumberOfItems)
{
    /**
    * For example your can generate a random number, and if that random
     number is < 0.5 then the item will be included in the knapsack, hence you
     will add 1, otherwise add 0.
     * Population size is determined by you. And you
       can actually try different population sizes to see how it will affect your
       results.*/


    float RandomNumber;
    Population=new int*[NumberOfPopulation];
    for(int i=0; i<NumberOfPopulation; i++)
        Population[i]=new int[NumberOfItems];

    for(int i=0; i<NumberOfPopulation; i++)
    {
        for(int j=0; j<NumberOfItems; j++)
        {
            RandomNumber=(float )(rand()%10)/10;
            if(RandomNumber<0.5)Population[i][j]=1;
            else Population[i][j]=0;
        }
    }

}


int getFitness(int **ItemPair, int Row, int Items)
{
    /**
     * the sum of the included items'weights
     * you need to add a condition to check if the chromosome is a
       feasible solution or not, if not you need to handle this case. How you will
       handle it, is up to you. Surprise us!
     */

    int FitnessVal=0;
    for(int i=0; i<Items; i++)
        FitnessVal+= (ItemPair[0][i]) * (Population[Row][i]);


    return FitnessVal;
}

int getWight(int **ItemPair, int Row, int Items)
{
    int FitnessVal=0;
    for(int i=0; i<Items; i++)
        FitnessVal+= (ItemPair[i][0]) * (Population[Row][i]);
    return FitnessVal;
}

void fillFitnessArray( int **ItemPairs, int Items) {

    FitnessArray = new int[NumberOfPopulation];
    for (int i = 0; i < NumberOfPopulation; i++) {

        FitnessArray[i] = getFitness(ItemPairs, i, Items);
    }
}

int selection(int **ItemPairs, int Items)
{
    /**
     * Roulette Wheel
     */
    int Total=0;
    fillFitnessArray( ItemPairs, Items);
    for(int i=0; i<NumberOfPopulation; i++)Total+=FitnessArray[i];
    double *RouletteWheel=new double[NumberOfPopulation];
    for(int i=0; i<NumberOfPopulation; i++)
    {
        if(i==0)RouletteWheel[i]=((double)FitnessArray[i]/(double)Total);
        else  RouletteWheel[i]=(((double)FitnessArray[i]/(double)Total)+RouletteWheel[i-1]);
    }
    srand(time(NULL));
    double GeneratedNumber=(double)(rand()%10)/10;
    for(int i=0; i<NumberOfPopulation; i++)
    {
        if(GeneratedNumber<= RouletteWheel[i])return i;  /** i is the selected index */
    }
    return -1; /** can not return 0 because it's handed in row 88*/
}

void crossover(int Parent_1Index, int Parent_2Index, int Items)
{
    /**
     * Crossover may happen or not according to the probability of crossover
     * Single point crossover after random  bit at all cases
     * using GGA replacement
     */
    srand(time(NULL));
    float RandomNumber;
    RandomNumber=(float)(rand()%10)/100;
    if(RandomNumber<0.07) {
        int PointIndex=rand()%Items;
        if(PointIndex!=0)PointIndex--;
        for (int i = PointIndex; i < Items; i++)
            swap(Population[Parent_1Index][i], Population[Parent_2Index][i]);
    }
    else return ;


}

void flipping(int &Number)
{
    if(Number==0)
        Number=1;
    else
        Number=0;
}
void mutation(int Items, int Row)
{
    srand(time(NULL));
    float RandomNumber;
        for(int j=0; j<Items; j++)
        {
           RandomNumber=(float)(rand()%10)/100;
           if(RandomNumber<0.05)flipping(Population[Row][j]);
        }

}
void makeFeasible(int **ItemPair, int Size,  int Row, int Items)
{
    int whight=getWight(ItemPair, Row, Items);
    int i=0;
    while(whight>Size)
    {
        if(i>=Items)i*=0;
        flipping(Population[Row][i]);
        whight=getFitness(ItemPair, Row, Items);
        i++;
    }
}
int main(void)
{
    int TestCases; cin>>TestCases;
    int Items;
    int Size;
    int selectedIndex_1;
    int selectedIndex_2;

    for(int i=0; i<TestCases; i++) {
        cin >> Items;
        cin >> Size;
        int **ItemPair= new int *[Items];
        for (int i = 0; i < Items; i++)
            ItemPair[i]=new int[2];

        for (int i = 0; i < Items; i++) {
            for (int j = 0; j < 2; j++)
                cin >> ItemPair[i][j];
        }

        generatePopuation(Items);

        for(int Generation=0; Generation < 1000; Generation++)
        {
            selectedIndex_1=selection(ItemPair, Items);
            selectedIndex_2=selection(ItemPair, Items);


            crossover( selectedIndex_1, selectedIndex_2,Items);
            mutation(Items, selectedIndex_1);
            mutation(Items, selectedIndex_2);


            makeFeasible(ItemPair, Size,  selectedIndex_1, Items);
            makeFeasible(ItemPair, Size,  selectedIndex_2, Items);

        }
        fillFitnessArray(ItemPair,Items);
        sort(FitnessArray, FitnessArray+NumberOfPopulation);
       cout<<"case: "<<i+1<<"  "<<FitnessArray[NumberOfPopulation-1]<<endl;

    }


    return 0;

}