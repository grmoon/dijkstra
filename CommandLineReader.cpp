//
//  CommandLineReader.cpp
//  dijkstra
//
//  Created by Gregory Moon & Tim Abbott on 2/3/16.
//  Copyright © 2016 Gregory Moon. All rights reserved.
//

#include "CommandLineReader.hpp"

//============================================================================================
// CommandLineReader::read
//
// Input:
//
// Output:
//  An InputReaderResult object.
//
// This method prompts the user for all of the input necessary to create a graph to be used with
// Dijksra's algorithm and outputs that graph.
//============================================================================================
InputReaderResult *CommandLineReader::read(){
    std::map<unsigned long, City *> cities = CommandLineReader::promptCities();
    unsigned long numExistingRoads, from, to;
    
    //prompt for existing roads
    numExistingRoads = CommandLineReader::promptRoads(false);
    CommandLineReader::promptRoadDetails(numExistingRoads, cities, false);
    printf("\n");
    
    //prompt for optional roads
    numExistingRoads = CommandLineReader::promptRoads(true);
    
    std::vector<Road *> optRoads;
    
    if(numExistingRoads > 0){
        optRoads = CommandLineReader::promptRoadDetails(numExistingRoads, cities, true);
    }
    
    printf("\n");
    
    from = CommandLineReader::promptLimit(cities.size(), true);
    printf("\n");
    
    to = CommandLineReader::promptLimit(cities.size(), false);
    printf("\n");
    
    return new InputReaderResult(to, from, cities, optRoads);
}

//============================================================================================
// CommandLineReader::promptRoadDetails
//
// Input:
//  numRoads:   The number of roads to create.
//  cities:     A map of city ids as the keys and City objects as the values.
//  optional:   A boolean value indicating whether or not the roads being input are optional or mandatory.
//
// Output:
//  A vector of Road objects.
//
// This method prompts the user for details concerning the edges of the graph.  The edges can be either optional
// or mandatory as indicated by the optional flag.
//============================================================================================
std::vector<Road *> CommandLineReader::promptRoadDetails(unsigned long numRoads, std::map<unsigned long, City *> cities, bool optional){
    std::vector<Road *> roads;
    unsigned long cityLimit = cities.size() - 1;
    const char *optionalText = CommandLineReader::getOptionalText(optional);
    Road *temp;
    
    if (numRoads > 0)
    {
        printf("  Enter the details for each road (fromCity:toCity:length):\n");
    }

    for (int i = 0; i < numRoads; i++) {
        printf("    Road %d: ", i);
        
        unsigned long from, to, length;
        scanf("%lu:%lu:%lu", &from, &to, &length);
        CommandLineReader::cleanBuffer();
        
        try{
            if(from > cityLimit || to > cityLimit){
                throw std::invalid_argument("");
            }
            
            temp = new Road(to, from, length, optional);
            roads.push_back(temp);
        }
        catch(std::invalid_argument& e){
            InputReader::logError("Values for 'fromCity' and 'toCity' must be unique integers between 0 and %lu inclusive and the value for 'length' must be a positive integer.", cityLimit);
            i--;
            continue;
        }
        
        if(!optional){
            cities[temp->from]->fromRoads.push_back(temp);
            cities[temp->to]->toRoads.push_back(temp);
        }
        
        printf("      New %s road from %lu to %lu with length %lu.\n", optionalText, temp->from, temp->to, temp->length);
    }
    
    return roads;
}

//============================================================================================
// CommandLineReader::promptCities
//
// Input:
//
// Output:
//  A map with city ids as keys and City objects as values.
//
// This method prompts the user for the number of nodes in the graph and creates City items
// that are inserted into each of those nodes.
//============================================================================================
std::map<unsigned long, City *> CommandLineReader::promptCities(){
    std::map<unsigned long, City *> cities;
    City *temp;
    int numCities;
    
    do{
        printf("How many cities are in your graph?: ");
        scanf("%d", &numCities);
        CommandLineReader::cleanBuffer();
        
        if(numCities < 2){
            InputReader::logError("There must be at least 2 cities in your graph.");
        }
    }while(numCities < 2);
    
    for (int i = 0; i < numCities; i++) {
        temp = new City(i);
        cities.insert(std::pair<int, City *>(i, temp));
    }
    
    printf("  %d cities (0 - %d) have been added.\n\n", numCities, numCities - 1);
    
    return cities;
}

//============================================================================================
// CommandLineReader::promptRoads
//
// Input:
//  optional:   A boolean value indicating whether or not the roads being input are optional or mandatory.
//
// Output:
//  An integer indicating how many roads are to be added.
//
// This method prompts the user for the number of roads that will follow as input.
//============================================================================================
unsigned long CommandLineReader::promptRoads(bool optional){
    unsigned long numRoads;
    const char *optionalText = CommandLineReader::getOptionalText(optional);
    
    printf("How many %s roads are in your graph?: ", optionalText);
    scanf("%lu", &numRoads);
    CommandLineReader::cleanBuffer();
    
    if(numRoads == ULONG_MAX){
        numRoads = 0;
        printf("  %lu %s roads (0 - %lu) are being added.\n\n", numRoads, optionalText, (numRoads > 0 ? numRoads - 1: 0));
    }
    
    return numRoads;
}

//============================================================================================
// CommandLineReader::promptLimit
//
// Input:
//  numCities:  The total number of cities in the graph.
//  start:      A boolean indicating whether the current limit is the start or end.
//
// Output:
//  An integer representing the city id of the starting or ending City object.
//
// This method prompts the user for details concerning the source or target of Dijkstra's algorithm.
//============================================================================================
unsigned long CommandLineReader::promptLimit(unsigned long numCities, bool start){
    unsigned long ret, cityLimit = numCities - 1;
    
    do{
        printf("What city do you want to %s in?: ", start ? "start" : "end");
        scanf("%lu", &ret);
        CommandLineReader::cleanBuffer();
        
        if(ret > cityLimit){
            InputReader::logError("The %s city must be an integer between 0 and %lu.", start ? "start" : "end", cityLimit);
        }
    }while(ret > cityLimit);
    
    printf("  %sing city: %lu\n\n", start ? "Start" : "End", ret);
    return ret;
}

//============================================================================================
// CommandLineReader::cleanBuffer
//
// Input:
//
// Output:
//
// A simply utiltiy for clearing the scanf buffer between user inputs.
//============================================================================================
void CommandLineReader::cleanBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
