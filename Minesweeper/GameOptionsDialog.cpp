#include "GameOptionsDialog.h"

#include <string>
#include <Windows.h>

#include "constants.h"
#include "GameWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

GameOptionsDialog::GameOptionsDialog() {}

/*
*	===========================
*	===== Private Methods =====
*	===========================
*/

void GameOptionsDialog::InitDifficultyList()
{
	WCHAR szTemp[constants::MAX_LOADSTRING];

	HWND hWndDifficultyList{ GetDlgItem(m_hDlg, IDC_DROPLIST_GAME_DIFFICULTY) };

	LoadString(GetModuleHandle(nullptr), IDS_BEGINNER_OPTION, szTemp, sizeof(szTemp) / sizeof(WCHAR));
	SendMessage(hWndDifficultyList, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szTemp));

	LoadString(GetModuleHandle(nullptr), IDS_INTERMEDIATE_OPTION, szTemp, sizeof(szTemp) / sizeof(WCHAR));
	SendMessage(hWndDifficultyList, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szTemp));

	LoadString(GetModuleHandle(nullptr), IDS_EXPERT_OPTION, szTemp, sizeof(szTemp) / sizeof(WCHAR));
	SendMessage(hWndDifficultyList, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szTemp));

	LoadString(GetModuleHandle(nullptr), IDS_CUSTOM_OPTION, szTemp, sizeof(szTemp) / sizeof(WCHAR));
	SendMessage(hWndDifficultyList, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szTemp));
}

/*
*	============================
*	===== Dialog Procedure =====
*	============================
*/

INT_PTR CALLBACK GameOptionsDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (!m_pGameWindow)
		{
			m_pGameWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(GetParent(m_hDlg), GWLP_USERDATA));
		}

		if (m_bQuestionMarksEnabled)
		{
			CheckDlgButton(m_hDlg, IDC_CHECK_QMARKS, BST_CHECKED);
		}

		InitDifficultyList();
		SendDlgItemMessage(m_hDlg, IDC_DROPLIST_GAME_DIFFICULTY, CB_SETCURSEL, m_uCurrentDifficultyIndex, 0);

		std::wstring szTemp{ std::to_wstring(m_uCurrentWidth) };
		SendDlgItemMessage(m_hDlg, IDC_EDIT_WIDTH, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(szTemp.data()));
		SendDlgItemMessage(m_hDlg, IDC_EDIT_WIDTH, EM_SETLIMITTEXT, 2, 0);

		szTemp = std::to_wstring(m_uCurrentHeight);
		SendDlgItemMessage(m_hDlg, IDC_EDIT_HEIGHT, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(szTemp.data()));
		SendDlgItemMessage(m_hDlg, IDC_EDIT_HEIGHT, EM_SETLIMITTEXT, 2, 0);

		szTemp = std::to_wstring(m_uCurrentMines);
		SendDlgItemMessage(m_hDlg, IDC_EDIT_MINES, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(szTemp.data()));
		SendDlgItemMessage(m_hDlg, IDC_EDIT_MINES, EM_SETLIMITTEXT, 4, 0);

		if (m_uCurrentDifficultyIndex == 3)
		{
			EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_WIDTH), TRUE);
			EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_HEIGHT), TRUE);
			EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_MINES), TRUE);
		}
	}
	return static_cast<INT_PTR>(TRUE);

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (UINT questionDialogChecked{ IsDlgButtonChecked(m_hDlg, IDC_CHECK_QMARKS) };
				m_bQuestionMarksEnabled != questionDialogChecked)
			{
				m_bQuestionMarksEnabled = questionDialogChecked;
				m_pGameWindow->ToggleQuestionMarkUsage();
			}

			m_uCurrentDifficultyIndex = SendDlgItemMessage(m_hDlg, IDC_DROPLIST_GAME_DIFFICULTY, CB_GETCURSEL, 0, 0);

			WCHAR szTemp[5]{};

			SendDlgItemMessage(m_hDlg, IDC_EDIT_WIDTH, WM_GETTEXT, 5, reinterpret_cast<LPARAM>(szTemp));
			m_uCurrentWidth = static_cast<UINT>(std::stoul(szTemp));

			if (m_uCurrentWidth > constants::MAX_FIELD_DIMENSION)
			{
				m_uCurrentWidth = constants::MAX_FIELD_DIMENSION;
			}
			else if (m_uCurrentWidth < 7)
			{
				m_uCurrentWidth = 7;
			}

			SendDlgItemMessage(m_hDlg, IDC_EDIT_HEIGHT, WM_GETTEXT, 5, reinterpret_cast<LPARAM>(szTemp));
			m_uCurrentHeight = static_cast<UINT>(std::stoul(szTemp));

			if (m_uCurrentHeight > constants::MAX_FIELD_DIMENSION)
			{
				m_uCurrentHeight = constants::MAX_FIELD_DIMENSION;
			}
			else if (m_uCurrentHeight < 1)
			{
				m_uCurrentHeight = 1;
			}

			SendDlgItemMessage(m_hDlg, IDC_EDIT_MINES, WM_GETTEXT, 5, reinterpret_cast<LPARAM>(szTemp));
			m_uCurrentMines = static_cast<UINT>(std::stoul(szTemp));

			if (m_uCurrentWidth * m_uCurrentHeight <= m_uCurrentMines)
			{
				m_uCurrentMines = m_uCurrentWidth * m_uCurrentHeight - 1;
			}
			else if (m_uCurrentMines < 1)
			{
				m_uCurrentMines = 1;
			}

			switch (m_uCurrentDifficultyIndex)
			{
			case 0:
				m_pGameWindow->ResizeMinefield(constants::BEGINNER_WIDTH, constants::BEGINNER_HEIGHT,
					constants::BEGINNER_CMINES);
				break;
			case 1:
				m_pGameWindow->ResizeMinefield(constants::INTERMEDIATE_WIDTH, constants::INTERMEDIATE_HEIGHT,
					constants::INTERMEDIATE_CMINES);
				break;
			case 2:
				m_pGameWindow->ResizeMinefield(constants::EXPERT_WIDTH, constants::EXPERT_HEIGHT,
					constants::EXPERT_CMINES);
				break;
			case 3:
				m_pGameWindow->ResizeMinefield(m_uCurrentWidth, m_uCurrentHeight, m_uCurrentMines);
				break;
			}
			EndDialog(m_hDlg, LOWORD(wParam));
		}
		return static_cast<INT_PTR>(TRUE);

		case IDCANCEL:
			EndDialog(m_hDlg, LOWORD(wParam));
			return static_cast<INT_PTR>(TRUE);

		case IDC_DROPLIST_GAME_DIFFICULTY:
			if (HIWORD(wParam) == CBN_SELENDOK)
			{
				switch (SendDlgItemMessage(m_hDlg, IDC_DROPLIST_GAME_DIFFICULTY, CB_GETCURSEL, 0, 0))
				{
				case 0:
					[[fallthrough]];
				case 1:
					[[fallthrough]];
				case 2:
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_WIDTH), FALSE);
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_HEIGHT), FALSE);
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_MINES), FALSE);
					break;
				case 3:
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_WIDTH), TRUE);
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_HEIGHT), TRUE);
					EnableWindow(GetDlgItem(m_hDlg, IDC_EDIT_MINES), TRUE);
					break;
				}
			}
			return static_cast<INT_PTR>(TRUE);
		}
	}
	return static_cast<INT_PTR>(FALSE);

	default:
		return static_cast<INT_PTR>(FALSE);
	}
}