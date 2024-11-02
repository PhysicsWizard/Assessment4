// Fill out your copyright notice in the Description page of Project Settings.


#include "Planner.h"
#include "Agent.h"       
#include "Action.h"      
#include "Goal.h"        
#include "WorldState.h"  
#include "Beliefs.h"     


TArray<UAction*> UPlanner::CreatePlan(UAgent* Agent, UGoal* Goal, UWorldState& WorldState,  UBeliefs& Beliefs)
{
	TArray<UAction*> AvailableActions = Agent -> GetAvailableAction();
	
	//UE_LOG(LogTemp, Warning, TEXT("Available Action Count: %d"), AvailableActions.Num());
	TArray<UAction*> ThePlan;
	 UWorldState* CurrentState = WorldState.Clone();
	UBeliefs* CurrentBeliefs = Beliefs.Clone();

	for(UAction* Action: AvailableActions)
	{
		bool bActionPossible = Action->IsActionPossible(*CurrentState, *CurrentBeliefs);
		UE_LOG(LogTemp, Warning, TEXT("Action %s possible: %s"), *Action->GetName(), bActionPossible ? TEXT("true") : TEXT("false"));
		if(Action->IsActionPossible(*CurrentState, *CurrentBeliefs))
		{
			ThePlan.Add(Action); 
			Action->ApplyEffects( *CurrentState);
		}

		if(Goal->IsGoalAchieved(*CurrentState, *CurrentBeliefs))
			return ThePlan;
		
	}
	
	if(ThePlan.Num() > 0)
	{
		return ThePlan;
	}
	
	return TArray<UAction*>();
}

TArray<UAction*> UPlanner::FindBestPlan(UAgent* Agent, UWorldState& WorldState, UBeliefs& Beliefs)
{
	TArray<UGoal*> Goals = Agent->GetGoals();
	TArray<UAction*> BestPlan;
	float BestPlanCost = FLT_MAX;

	for (UGoal* Goal : Goals)
	{
		if (!Goal->IsGoalRelevant(WorldState, Beliefs))
		{
			continue;
		}

		// Use a planning algorithm like A* here to find the best plan
		TArray<UAction*> CurrentPlan = CreatePlan(Agent, Goal, WorldState, Beliefs);

		if (CurrentPlan.Num() > 0)
		{
			float CurrentPlanCost = 0.0f;
			for (UAction* Action : CurrentPlan)
			{
				CurrentPlanCost += Action->Getcost();
			}

			if (CurrentPlanCost < BestPlanCost)
			{
				BestPlanCost = CurrentPlanCost;
				BestPlan = CurrentPlan;
				Agent->CurrentGoal = Goal;  
			}
		}
	}
	return BestPlan;
} 
