#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DeathMenuWidget.generated.h"

UCLASS()
class LABIRINTHORROR_API UDeathMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // Ссылка на вашего персонажа
    UPROPERTY(BlueprintReadOnly, Category = "Player")
        class ALabirintHorrorCharacter* PlayerCharacter;

    // Метод для респавна
    UFUNCTION(BlueprintCallable, Category = "Actions")
        void Respawn();

protected:
    UPROPERTY(meta = (BindWidget))
        class UButton* RespawnButton;
private:
    UFUNCTION()
        void OnRespawnButtonClicked();
};
