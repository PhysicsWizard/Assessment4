// Fill out your copyright notice in the Description page of Project Settings.


#include "Planner.h"
#include "Agent.h"       
#include "Action.h"      
#include "Goal.h"        
#include "WorldState.h"  
#include "Beliefs.h"     


TArray<UAction*> UPlanner::CreatePlan(UAgent* Agent, UGoal* Goal, UWorldState& WorldState,  UBeliefs& Beliefs)
{
	TArray<UAction*> AvailableActions = Agent->GetAvailableAction();
	TArray<UAction*> ThePlan;
	UWorldState* CurrentState = WorldState.Clone();
	UBeliefs* CurrentBeliefs = Beliefs.Clone();


	int32 Index = 0;
	while (Index < AvailableActions.Num())
	{
		UAction* Action = AvailableActions[Index];
		bool bActionPossible = Action->IsActionPossible(*CurrentState, *CurrentBeliefs);
		UE_LOG(LogTemp, Warning, TEXT("Action %s possible: %s"), *Action->GetName(), bActionPossible ? TEXT("true") : TEXT("false"));
        
		if (bActionPossible)
		{
			ThePlan.Add(Action);
			AvailableActions.RemoveAt(Index);
			Action->ApplyEffects(*CurrentState);
			if (Goal->IsGoalAchieved(*CurrentState, *CurrentBeliefs))
			{
				return ThePlan;
			}
		}
		else
		{
			++Index; // Move to the next action if this action isn't possible
		}
	}

	if (ThePlan.Num() > 0)
	{
		return ThePlan;
	}

	// Return an empty plan if no solution is found
	UE_LOG(LogTemp, Warning, TEXT("No valid plan could be created to achieve the goal."));
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
