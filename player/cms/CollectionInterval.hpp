#pragma once

#include "CmsStatus.hpp"
#include "RequiredFilesDownloader.hpp"

#include "cms/xmds/NotifyStatus.hpp"
#include "cms/xmds/RegisterDisplay.hpp"
#include "cms/xmds/RequiredFiles.hpp"
#include "cms/xmds/Schedule.hpp"
#include "cms/xmds/SubmitLog.hpp"
#include "cms/xmds/SubmitStats.hpp"
#include "networking/ResponseResult.hpp"
#include "schedule/LayoutSchedule.hpp"

#include "common/JoinableThread.hpp"
#include "common/dt/Timer.hpp"
#include "common/fs/FilePath.hpp"

#include <boost/signals2/signal.hpp>

using CollectionResultCallback = std::function<void(const PlayerError&)>;
using SignalSettingsUpdated = boost::signals2::signal<void(const PlayerSettings&)>;
using SignalScheduleAvailable = boost::signals2::signal<void(LayoutSchedule)>;
using SignalCollectionFinished = boost::signals2::signal<void(const PlayerError&)>;
using SignalFilesDownloaded = boost::signals2::signal<void()>;

class XmdsRequestSender;
class StatsRecorder;
class FileCache;

class CollectionInterval
{
    static constexpr const uint DefaultInterval = 900;

public:
    CollectionInterval(XmdsRequestSender& xmdsSender,
                       StatsRecorder& statsRecorder,
                       FileCache& fileCache,
                       const FilePath& resourceDirectory);

    bool running() const;
    void stop();
    void collectNow();
    void updateInterval(int collectInterval);
    CmsStatus status() const;

    SignalSettingsUpdated& settingsUpdated();
    SignalScheduleAvailable& scheduleAvailable();
    SignalCollectionFinished& collectionFinished();
    SignalFilesDownloaded& filesDownloaded();

    void setCurrentLayoutId(const LayoutId& currentLayoutId);

private:
    void startTimer();
    void sessionFinished(const PlayerError& = {});

    void onDisplayRegistered(const ResponseResult<RegisterDisplay::Result>& registerDisplay);
    PlayerError displayStatus(const RegisterDisplay::Result::Status& status);
    void onRequiredFiles(const ResponseResult<RequiredFiles::Result>& requiredFiles);
    void updateMediaInventory(const RequiredFiles::Result& requiredFilesResult);
    void onSchedule(const ResponseResult<Schedule::Result>& schedule);
    void onSubmitStats(const ResponseResult<SubmitStats::Result>& statsResult);
    template <typename Result>
    void onSubmitted(std::string_view requestName, const ResponseResult<Result>& submitResult);

private:
    XmdsRequestSender& xmdsSender_;
    StatsRecorder& statsRecorder_;
    FileCache& fileCache_;
    std::unique_ptr<JoinableThread> workerThread_;
    std::unique_ptr<Timer> intervalTimer_;
    std::atomic_int collectInterval_;
    std::atomic_bool running_;
    CmsStatus status_;
    LayoutId currentLayoutId_;
    FilePath resourceDirectory_;
    SignalSettingsUpdated settingsUpdated_;
    SignalScheduleAvailable scheduleAvailable_;
    SignalCollectionFinished collectionFinished_;
    SignalFilesDownloaded filesDownloaded_;
};
