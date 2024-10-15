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
	TArray<UGoal*> Goals =  Agent->GetGoals();
	//UE_LOG(LogTemp, Warning, TEXT("Goals Count: %d"), Goals.Num());
	TArray<UAction*> BestPlan;
	float BestPlanCost = FLT_MAX;

	for(UGoal* Goal : Goals)
	{
		TArray<UAction*> CurrentPlan = CreatePlan(Agent, Goal, WorldState, Beliefs);
		//UE_LOG(LogTemp, Warning, TEXT("Available Action Count: %d"), CurrentPlan.Num());
		if(CurrentPlan.Num()>0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Current Plan has an action"));
			float CurrentPlanCost = 0.0f;
			for (UAction* Action : CurrentPlan)
			{
				CurrentPlanCost += Action->Getcost();
			}

			if(CurrentPlanCost < BestPlanCost && CurrentPlan.Num() > 0)
			{
				BestPlanCost = CurrentPlanCost;
				BestPlan = CurrentPlan;
			}
		}
	}
	return BestPlan;
} 
