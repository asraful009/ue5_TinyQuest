#include "TQQuest.h"

bool FTQQuest::AreRequiredObjectivesComplete() const
{
    if (State == ETQQuestState::NotStarted || State == ETQQuestState::Canceled)
    {
        return false;
    }
    if (State == ETQQuestState::Completed)
    {
        return true;
    }

    for (const FTQQuestObjective& Objective : Objectives)
    {
        if (!Objective.IsOptional && !Objective.IsComplete)
        {
            return false;
        }
    }
    return true;
}

bool FTQQuest::AreAllObjectivesComplete() const
{
    if (State == ETQQuestState::NotStarted || State == ETQQuestState::Canceled)
    {
        return false;
    }
    if (State == ETQQuestState::Completed)
    {
        return true;
    }

    for (const FTQQuestObjective& Objective : Objectives)
    {
        if (!Objective.IsComplete)
        {
            return false;
        }
    }
    return true;
}

int32 FTQQuest::FindObjectiveIndex(ETQQuestType Type, const FGameplayTag& TargetTag) const
{
    for (int32 i = 0; i < Objectives.Num(); ++i)
    {
        const FTQQuestObjective& Objective = Objectives[i];

        if (Objective.QuestObjectiveType == Type && Objective.TargetTag == TargetTag && !Objective.IsComplete)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

bool FTQQuest::AddProgressToObjective(ETQQuestType Type, const FGameplayTag& TargetTag, int32 Amount)
{
    const int32 Index = FindObjectiveIndex(Type, TargetTag);
    if (Index == INDEX_NONE)
    {
        return false;
    }
    Objectives[Index].AddProgress(Amount);
    return true;
}

void FTQQuest::ResetQuest()
{
    for (FTQQuestObjective& Objective : Objectives)
    {
        Objective.ResetProgress();
    }
    State = ETQQuestState::NotStarted;
}
