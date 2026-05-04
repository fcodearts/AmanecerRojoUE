#include "AmanecerPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Dialogue/DialogueSubsystem.h"

AAmanecerPlayerController::AAmanecerPlayerController()
{
    bShowMouseCursor = false;
}

void AAmanecerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ContextoMovimiento)
    {
        if (auto* InputSub = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            InputSub->AddMappingContext(ContextoMovimiento, 0);
    }
}

void AAmanecerPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    auto* EIC = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EIC) return;

    if (AccionMover)       EIC->BindAction(AccionMover,       ETriggerEvent::Triggered, this, &AAmanecerPlayerController::OnMover);
    if (AccionAtacar)      EIC->BindAction(AccionAtacar,      ETriggerEvent::Started,   this, &AAmanecerPlayerController::OnAtacar);
    if (AccionInteractuar) EIC->BindAction(AccionInteractuar, ETriggerEvent::Started,   this, &AAmanecerPlayerController::OnInteractuar);
}

void AAmanecerPlayerController::OnMover(const FInputActionValue& Value)
{
    if (GetPawn())
    {
        FVector2D Dir = Value.Get<FVector2D>();
        // Movimiento isométrico: convertir input 2D a dirección 3D
        FVector Forward = FVector(Dir.Y, Dir.X, 0.f).GetSafeNormal();
        GetPawn()->AddMovementInput(Forward);
    }
}

void AAmanecerPlayerController::OnAtacar(const FInputActionValue& Value)
{
    // El PlayerPawn implementa la lógica de ataque — notificamos via tag
    // La conexión real se hace en BP_Darrow que hereda de este controller
    if (APawn* Pawn = GetPawn())
        Pawn->NotifyActorBeginCursorOver(); // placeholder hasta tener la interfaz IAtacable
}

void AAmanecerPlayerController::OnInteractuar(const FInputActionValue& Value)
{
    // Avanzar diálogo o interactuar con trigger
    if (auto* Dialogue = GetGameInstance()->GetSubsystem<UDialogueSubsystem>())
    {
        if (Dialogue->EstaActivo())
            Dialogue->Avanzar();
    }
}
