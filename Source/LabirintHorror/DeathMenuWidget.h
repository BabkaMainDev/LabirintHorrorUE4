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

    // ������ �� ������ ���������
    UPROPERTY(BlueprintReadOnly, Category = "Player")
        class ALabirintHorrorCharacter* PlayerCharacter;

    // ����� ��� ��������
    UFUNCTION(BlueprintCallable, Category = "Actions")
        void Respawn();

protected:
    UPROPERTY(meta = (BindWidget))
        class UButton* RespawnButton;
private:
    UFUNCTION()
        void OnRespawnButtonClicked();
};
