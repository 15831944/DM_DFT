/* -LICENSE-START-
** Copyright (c) 2013 Blackmagic Design
**
** Permission is hereby granted, free of charge, to any person or organization
** obtaining a copy of the software and accompanying documentation covered by
** this license (the "Software") to use, reproduce, display, distribute,
** execute, and transmit the Software, and to prepare derivative works of the
** Software, and to permit third-parties to whom the Software is furnished to
** do so, all subject to the following:
** 
** The copyright notices in the Software and this entire statement, including
** the above license grant, this restriction and the following disclaimer,
** must be included in all copies of the Software, in whole or in part, and
** all derivative works of the Software, unless such copies or derivative
** works are solely in the form of machine-executable object code generated by
** a source language processor.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
** -LICENSE-END-
*/

#include <vector>
#include "DeckLinkAPI_h.h"
//#include "CapturePreview.h"
//#include "CapturePreviewDlg.h"
#include "AcqVoltageSamples_IntClk.h"
#include "AcqVoltageSamples_IntClkDlg.h"
#include "PreviewWindow.h"


class DeckLinkDevice : public IDeckLinkInputCallback
{
private:

	CAcqVoltageSamples_IntClkDlg*					m_uiDelegate;
	ULONG								m_refCount;
	//
	CString								m_deviceName;
	IDeckLink*							m_deckLink;
	IDeckLinkInput*						m_deckLinkInput;
	std::vector<IDeckLinkDisplayMode*>	m_modeList;
	BOOL								m_supportsFormatDetection;
	bool								m_currentlyCapturing;
	bool								m_applyDetectedInputMode;
	//
	static void							GetAncillaryDataFromFrame(IDeckLinkVideoInputFrame* frame, BMDTimecodeFormat format, CString* timecodeString, CString* userBitsString);

public:
	DeckLinkDevice(CAcqVoltageSamples_IntClkDlg* uiDelegate, IDeckLink* device);
	virtual ~DeckLinkDevice();

	bool								Init();
	const CString&						GetDeviceName() { return m_deviceName; };
	bool								IsCapturing() { return m_currentlyCapturing; };
	bool								SupportsFormatDetection() { return (m_supportsFormatDetection == TRUE); };
	void			 					GetDisplayModeNames(std::vector<CString>& modeNames);
	bool								StartCapture(unsigned int videoModeIndex, IDeckLinkScreenPreviewCallback* screenPreviewCallback, bool applyDetectedInputMode);
	void								StopCapture();
	IDeckLink*							DeckLinkInstance() { return m_deckLink; }

	// IUnknown interface
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface (REFIID iid, LPVOID *ppv);
	virtual ULONG	STDMETHODCALLTYPE	AddRef ();
	virtual ULONG	STDMETHODCALLTYPE	Release ();

	// IDeckLinkInputCallback interface
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFormatChanged (/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newDisplayMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags);
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFrameArrived (/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket);
};

class DeckLinkDeviceDiscovery :  public IDeckLinkDeviceNotificationCallback
{
private:
	IDeckLinkDiscovery*					m_deckLinkDiscovery;
	CAcqVoltageSamples_IntClkDlg*					m_uiDelegate;
	ULONG								m_refCount;
public:
	DeckLinkDeviceDiscovery(CAcqVoltageSamples_IntClkDlg* uiDelegate);
	virtual ~DeckLinkDeviceDiscovery();

	bool						        Enable();
	void						        Disable();

	// IDeckLinkDeviceNotificationCallback interface
	virtual HRESULT	STDMETHODCALLTYPE	DeckLinkDeviceArrived (/* in */ IDeckLink* deckLink);
	virtual HRESULT	STDMETHODCALLTYPE	DeckLinkDeviceRemoved (/* in */ IDeckLink* deckLink);

	// IUnknown needs only a dummy implementation
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface (REFIID iid, LPVOID *ppv);
	virtual ULONG	STDMETHODCALLTYPE	AddRef ();
	virtual ULONG	STDMETHODCALLTYPE	Release ();
};
