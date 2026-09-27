#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TQQuestObjective.h"
#include "TQQuestTypes.h"
#include "TQQuest.generated.h"

USTRUCT(BlueprintType)
struct FTQQuest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FText QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FText QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FName PrerequisiteQuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    TArray<FTQQuestObjective> Objectives;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest|Runtime")
    ETQQuestState State = ETQQuestState::NotStarted;
    
    /** True if every non-optional objective has IsComplete == true. */
    bool AreRequiredObjectivesComplete() const;

    /** True if every objective (optional included) is complete. */
    bool AreAllObjectivesComplete() const;

    /** Returns index of the first incomplete objective matching Type + Tag, or INDEX_NONE. */
    int32 FindObjectiveIndex(ETQQuestType Type, const FGameplayTag& TargetTag) const;

    // ---- Mutations ----

    /** Applies progress to the first matching incomplete objective. Returns true if applied. */
    bool AddProgressToObjective(ETQQuestType Type, const FGameplayTag& TargetTag, int32 Amount = 1);

    /** Resets all runtime state back to initial values. */
    void ResetQuest();
};
