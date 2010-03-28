// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class _Application : public COleDispatchDriver
{
public:
	_Application() {}		// Calls COleDispatchDriver default constructor
	_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetPresentations();
	LPDISPATCH GetWindows();
	LPDISPATCH GetActiveWindow();
	LPDISPATCH GetActivePresentation();
	LPDISPATCH GetSlideShowWindows();
	LPDISPATCH GetCommandBars();
	CString GetPath();
	CString GetName();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetAssistant();
	LPDISPATCH GetFileSearch();
	LPDISPATCH GetFileFind();
	CString GetBuild();
	CString GetVersion();
	CString GetOperatingSystem();
	CString GetActivePrinter();
	long GetCreator();
	LPDISPATCH GetAddIns();
	// method 'GetVbe' not emitted because of invalid return type or parameter type
	void Help(LPCTSTR HelpFile, long ContextID);
	void Quit();
	// method 'Run' not emitted because of invalid return type or parameter type
	float GetLeft();
	void SetLeft(float newValue);
	float GetTop();
	void SetTop(float newValue);
	float GetWidth();
	void SetWidth(float newValue);
	float GetHeight();
	void SetHeight(float newValue);
	long GetWindowState();
	void SetWindowState(long nNewValue);
	long GetVisible();
	void SetVisible(long nNewValue);
	long GetActive();
	void Activate();
	LPDISPATCH GetAnswerWizard();
	LPDISPATCH GetCOMAddIns();
	CString GetProductCode();
	LPDISPATCH GetDefaultWebOptions();
	LPDISPATCH GetLanguageSettings();
	long GetShowWindowsInTaskbar();
	void SetShowWindowsInTaskbar(long nNewValue);
	long GetFeatureInstall();
	void SetFeatureInstall(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow wrapper class

class SlideShowWindow : public COleDispatchDriver
{
public:
	SlideShowWindow() {}		// Calls COleDispatchDriver default constructor
	SlideShowWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowWindow(const SlideShowWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetView();
	LPDISPATCH GetPresentation();
	long GetIsFullScreen();
	float GetLeft();
	void SetLeft(float newValue);
	float GetTop();
	void SetTop(float newValue);
	float GetWidth();
	void SetWidth(float newValue);
	float GetHeight();
	void SetHeight(float newValue);
	long GetActive();
	void Activate();
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowView wrapper class

class SlideShowView : public COleDispatchDriver
{
public:
	SlideShowView() {}		// Calls COleDispatchDriver default constructor
	SlideShowView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowView(const SlideShowView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	long GetZoom();
	LPDISPATCH GetSlide();
	long GetPointerType();
	void SetPointerType(long nNewValue);
	long GetState();
	void SetState(long nNewValue);
	long GetAcceleratorsEnabled();
	void SetAcceleratorsEnabled(long nNewValue);
	float GetPresentationElapsedTime();
	float GetSlideElapsedTime();
	void SetSlideElapsedTime(float newValue);
	LPDISPATCH GetLastSlideViewed();
	long GetAdvanceMode();
	LPDISPATCH GetPointerColor();
	long GetIsNamedShow();
	CString GetSlideShowName();
	void DrawLine(float BeginX, float BeginY, float EndX, float EndY);
	void EraseDrawing();
	void First();
	void Last();
	void Next();
	void Previous();
	void GotoSlide(long index, long ResetSlide);
	void GotoNamedShow(LPCTSTR SlideShowName);
	void EndNamedShow();
	void ResetSlideTime();
	void Exit();
	long GetCurrentShowPosition();
};
/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings wrapper class

class SlideShowSettings : public COleDispatchDriver
{
public:
	SlideShowSettings() {}		// Calls COleDispatchDriver default constructor
	SlideShowSettings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SlideShowSettings(const SlideShowSettings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetPointerColor();
	LPDISPATCH GetNamedSlideShows();
	long GetStartingSlide();
	void SetStartingSlide(long nNewValue);
	long GetEndingSlide();
	void SetEndingSlide(long nNewValue);
	long GetAdvanceMode();
	void SetAdvanceMode(long nNewValue);
	LPDISPATCH Run();
	long GetLoopUntilStopped();
	void SetLoopUntilStopped(long nNewValue);
	long GetShowType();
	void SetShowType(long nNewValue);
	long GetShowWithNarration();
	void SetShowWithNarration(long nNewValue);
	long GetShowWithAnimation();
	void SetShowWithAnimation(long nNewValue);
	CString GetSlideShowName();
	void SetSlideShowName(LPCTSTR lpszNewValue);
	long GetRangeType();
	void SetRangeType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Presentations wrapper class

class Presentations : public COleDispatchDriver
{
public:
	Presentations() {}		// Calls COleDispatchDriver default constructor
	Presentations(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Presentations(const Presentations& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& index);
	LPDISPATCH Add(long WithWindow);
	LPDISPATCH Open(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class

class PageSetup : public COleDispatchDriver
{
public:
	PageSetup() {}		// Calls COleDispatchDriver default constructor
	PageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PageSetup(const PageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	long GetFirstSlideNumber();
	void SetFirstSlideNumber(long nNewValue);
	float GetSlideHeight();
	void SetSlideHeight(float newValue);
	float GetSlideWidth();
	void SetSlideWidth(float newValue);
	long GetSlideSize();
	void SetSlideSize(long nNewValue);
	long GetNotesOrientation();
	void SetNotesOrientation(long nNewValue);
	long GetSlideOrientation();
	void SetSlideOrientation(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Slides wrapper class

class Slides : public COleDispatchDriver
{
public:
	Slides() {}		// Calls COleDispatchDriver default constructor
	Slides(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Slides(const Slides& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH Item(const VARIANT& index);
	LPDISPATCH FindBySlideID(long SlideID);
	LPDISPATCH Add(long index, long Layout);
	long InsertFromFile(LPCTSTR FileName, long index, long SlideStart, long SlideEnd);
	LPDISPATCH Range(const VARIANT& index);
	LPDISPATCH Paste(long index);
};
/////////////////////////////////////////////////////////////////////////////
// _Slide wrapper class

class _Slide : public COleDispatchDriver
{
public:
	_Slide() {}		// Calls COleDispatchDriver default constructor
	_Slide(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Slide(const _Slide& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetShapes();
	LPDISPATCH GetHeadersFooters();
	LPDISPATCH GetSlideShowTransition();
	LPDISPATCH GetColorScheme();
	void SetColorScheme(LPDISPATCH newValue);
	LPDISPATCH GetBackground();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	long GetSlideID();
	long GetPrintSteps();
	void Select();
	void Cut();
	void Copy();
	long GetLayout();
	void SetLayout(long nNewValue);
	LPDISPATCH Duplicate();
	void Delete();
	LPDISPATCH GetTags();
	long GetSlideIndex();
	long GetSlideNumber();
	long GetDisplayMasterShapes();
	void SetDisplayMasterShapes(long nNewValue);
	long GetFollowMasterBackground();
	void SetFollowMasterBackground(long nNewValue);
	LPDISPATCH GetNotesPage();
	LPDISPATCH GetMaster();
	LPDISPATCH GetHyperlinks();
	void Export(LPCTSTR FileName, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight);
	LPDISPATCH GetScripts();
};
/////////////////////////////////////////////////////////////////////////////
// _Presentation wrapper class

class _Presentation : public COleDispatchDriver
{
public:
	_Presentation() {}		// Calls COleDispatchDriver default constructor
	_Presentation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Presentation(const _Presentation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	LPDISPATCH GetParent();
	LPDISPATCH GetSlideMaster();
	LPDISPATCH GetTitleMaster();
	long GetHasTitleMaster();
	LPDISPATCH AddTitleMaster();
	void ApplyTemplate(LPCTSTR FileName);
	CString GetTemplateName();
	LPDISPATCH GetNotesMaster();
	LPDISPATCH GetHandoutMaster();
	LPDISPATCH GetSlides();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetColorSchemes();
	LPDISPATCH GetExtraColors();
	LPDISPATCH GetSlideShowSettings();
	LPDISPATCH GetFonts();
	LPDISPATCH GetWindows();
	LPDISPATCH GetTags();
	LPDISPATCH GetDefaultShape();
	LPDISPATCH GetBuiltInDocumentProperties();
	LPDISPATCH GetCustomDocumentProperties();
	// method 'GetVBProject' not emitted because of invalid return type or parameter type
	long GetReadOnly();
	CString GetFullName();
	CString GetName();
	CString GetPath();
	long GetSaved();
	void SetSaved(long nNewValue);
	long GetLayoutDirection();
	void SetLayoutDirection(long nNewValue);
	LPDISPATCH NewWindow();
	void FollowHyperlink(LPCTSTR Address, LPCTSTR SubAddress, BOOL NewWindow, BOOL AddHistory, LPCTSTR ExtraInfo, long Method, LPCTSTR HeaderInfo);
	void AddToFavorites();
	LPDISPATCH GetPrintOptions();
	void PrintOut(long From, long To, LPCTSTR PrintToFile, long Copies, long Collate);
	void Save();
	void SaveAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts);
	void SaveCopyAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts);
	void Export(LPCTSTR Path, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight);
	void Close();
	LPDISPATCH GetContainer();
	long GetDisplayComments();
	void SetDisplayComments(long nNewValue);
	long GetFarEastLineBreakLevel();
	void SetFarEastLineBreakLevel(long nNewValue);
	CString GetNoLineBreakBefore();
	void SetNoLineBreakBefore(LPCTSTR lpszNewValue);
	CString GetNoLineBreakAfter();
	void SetNoLineBreakAfter(LPCTSTR lpszNewValue);
	void UpdateLinks();
	LPDISPATCH GetSlideShowWindow();
	long GetFarEastLineBreakLanguage();
	void SetFarEastLineBreakLanguage(long nNewValue);
	void WebPagePreview();
	long GetDefaultLanguageID();
	void SetDefaultLanguageID(long nNewValue);
	LPDISPATCH GetCommandBars();
	LPDISPATCH GetPublishObjects();
	LPDISPATCH GetWebOptions();
	LPDISPATCH GetHTMLProject();
	void ReloadAs(long cp);
	long GetEnvelopeVisible();
	void SetEnvelopeVisible(long nNewValue);
	long GetVBASigned();
};
