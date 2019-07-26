﻿#include "cqwin32file.h"
#include <shlobj.h>

LPCWSTR CQDocumentDirectoryW(VOID)
{
    WCHAR pszPath[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath) == S_OK)
    {
        return cq_store_wstr(pszPath);
    }
    else
    {
        return cq_store_wstr(NULL);
    }
}

LPCSTR CQDocumentDirectoryA(VOID)
{
    cq_malloc_pool
    {
        LPCWSTR z = CQDocumentDirectoryW();
        return CQMBStringFormWS(z);
    }
    cq_free_pool();
}

LPCWSTR CQCachesDirectoryW(VOID)
{
    WCHAR pszPath[MAX_PATH];
    if (GetTempPathW(cq_array_count(pszPath), pszPath) > 0)
    {
        return cq_store_wstr(pszPath);
    }
    else
    {
        return cq_store_wstr(NULL);
    }
}

LPCSTR CQCachesDirectoryA(VOID)
{
    cq_malloc_pool
    {
        LPCWSTR z = CQCachesDirectoryW();
        return CQMBStringFormWS(z);
    }
    cq_free_pool();
}

LPCWSTR CQTemporaryDirectoryW(VOID)
{
    return CQCachesDirectoryW();
}

LPCSTR CQTemporaryDirectoryA(VOID)
{
    return CQCachesDirectoryA();
}

static BOOL CQDiskItemExistsW(LPCWSTR pszPath, BOOL* bDirectory)
{
    if (!cq_wstr_empty(pszPath))
    {
        DWORD dwAttributes = GetFileAttributesW(pszPath);
        if (dwAttributes != INVALID_FILE_ATTRIBUTES)
        {
            *bDirectory = (dwAttributes & FILE_ATTRIBUTE_DIRECTORY);
            return TRUE;
        }
        else
        {
            *bDirectory = FALSE;
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

BOOL CQDirectoryExistsW(LPCWSTR pszPath)
{
    BOOL bDirectory = FALSE;
    BOOL bExists = CQDiskItemExistsW(pszPath, &bDirectory);
    return bExists && bDirectory;
}

BOOL CQDirectoryExistsA(LPCSTR pszPath)
{
    LPCWSTR a = CQWideStringFromMBS(pszPath);
    return CQDirectoryExistsW(a);
}

BOOL CQFileExistsW(LPCWSTR pszPath)
{
    BOOL bDirectory = FALSE;
    BOOL bExists = CQDiskItemExistsW(pszPath, &bDirectory);
    return bExists && !bDirectory;
}

BOOL CQFileExistsA(LPCSTR pszPath)
{
    LPCWSTR a = CQWideStringFromMBS(pszPath);
    return CQFileExistsW(a);
}

BOOL CQCreateDirectoryW(LPCWSTR pszPath, BOOL bIntermediate)
{
    if (cq_wstr_empty(pszPath))
    {
        return FALSE;
    }

    if (bIntermediate)
    {
        //SHCreateDirectory() isn't support relative path.
        if (pszPath[1] != L':')
        {
            WCHAR pszAllPath[MAX_PATH];
            GetCurrentDirectoryW(cq_array_count(pszAllPath), pszAllPath);
            wcscat_s(pszAllPath, cq_array_count(pszAllPath), L"\\");
            wcscat_s(pszAllPath, cq_array_count(pszAllPath), pszPath);
            return SHCreateDirectoryExW(NULL, pszAllPath, NULL) == ERROR_SUCCESS;
        }
        else
        {
            return SHCreateDirectoryExW(NULL, pszPath, NULL) == ERROR_SUCCESS;
        }
    }
    else
    {
        return CreateDirectoryW(pszPath, NULL);
    }
}

BOOL CQCreateDirectoryA(LPCSTR pszPath, BOOL bIntermediate)
{
    LPCWSTR a = CQWideStringFromMBS(pszPath);
    BOOL    b = bIntermediate;

    return CQCreateDirectoryW(a, b);
}

VOID CQRemovePathW(LPCWSTR pszPath)
{
    if (cq_wstr_empty(pszPath))
    {
        return;
    }

    cq_malloc_pool
    {
        //SHFILEOPSTRUCT.pFrom need double null terminating.
        size_t nPath = wcslen(pszPath);
        LPWSTR pszFromPath = cq_push_array(sizeof(WCHAR), nPath + 2);
        wcscpy(pszFromPath, pszPath);
        pszFromPath[nPath + 1] = L'\0';

        SHFILEOPSTRUCTW stOperation;
        memset(&stOperation, 0, sizeof(stOperation));
        stOperation.pFrom = pszFromPath;
        stOperation.pTo = NULL;
        stOperation.wFunc = FO_DELETE;
        stOperation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
        SHFileOperation(&stOperation);
    }
    cq_free_pool();
}

VOID CQRemovePathA(LPCSTR pszPath)
{
    LPCWSTR a = CQWideStringFromMBS(pszPath);
    CQRemovePathW(a);
}
