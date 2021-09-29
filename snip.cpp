// Wait events
void UnrealSnippets::WaitEvents()
{
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle,
    [this]
    {
        // Do some work here

        FTimerHandle TimerHandle_Fade;
        GetWorldTimerManager().SetTimer(TimerHandle_Fade,
        [this]
        {
            // Do more work there
        },
        1.0f, false);

    },
    3.0f, false);
}
////////////////////////////////////////////////////

// Async tasks
void UnrealSnippets::RunAsyncTask()
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, dataToSave, UserIndex]()
        {
            UGameplayStatics::SaveDataToSlot(dataToSave.SaveDataBytes, GetPlatformSaveSlotName(dataToSave.SaveDataName), UserIndex);
            UE_LOG(DreadOutLog_SaveGame, Warning, TEXT("RunSaveQueue... SaveDataToSlot"));

            SaveCriticalSection.Lock();
            SaveInProgress = false;
            SaveCriticalSection.Unlock();

            AsyncTask(ENamedThreads::GameThread, [this, dataToSave]()
                {
                    TArray<uint8> result;

                    for (int32 i = 0; i < dataToSave.CallbackFunctions.Num(); i++)
                    {
                        if (dataToSave.CallbackFunctions[i] != nullptr)
                        {
                            dataToSave.CallbackFunctions[i](result);
                        }
                    }
                });
        });
}

void UnrealSnippets::StartAsyncTask()
{
    AddToSaveDataQueue(ESaveType::Load, GetPlatformSaveSlotName(GetGameDataSlot(EGameDataType::Options)), inData, [this](TArray<uint8> saveData)
    {
        // Do work that will run once the save data is added to the queue

    });
}
////////////////////////////////////////