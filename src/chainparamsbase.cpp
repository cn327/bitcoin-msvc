// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparamsbase.h"

#include "tinyformat.h"
#include "util.h"

#include <assert.h>
#include <algorithm>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// Network type and each name
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
static std::map<NetworkType, std::string> _NetworkTypeNames = {
    { NETWORK_MAIN, "main" },
    { NETWORK_TESTNET, "test" },
    { NETWORK_REGTEST, "regtest" },
};
std::string NetworkType2String(NetworkType networkType)
{
    return _NetworkTypeNames[networkType];
}
NetworkType String2NetworkType(const std::string& networkTypeName)
{
    auto itr = find_if(_NetworkTypeNames.begin(), _NetworkTypeNames.end(),
        [networkTypeName](const std::pair<NetworkType, std::string>& s) { return s.second == networkTypeName; });
    return itr->first;
}
/*
const std::string CBaseChainParams::MAIN = "main";
const std::string CBaseChainParams::TESTNET = "test";
const std::string CBaseChainParams::REGTEST = "regtest";
*/

void AppendParamsHelpMessages(std::string& strUsage, bool debugHelp)
{
    strUsage += HelpMessageGroup(_("Chain selection options:"));
    strUsage += HelpMessageOpt("-testnet", _("Use the test chain"));
    if (debugHelp) {
        strUsage += HelpMessageOpt("-regtest", "Enter regression test mode, which uses a special chain in which blocks can be solved instantly. "
                                   "This is intended for regression testing tools and app development.");
    }
}

/**
 * Main network
 */
class CBaseMainParams : public CBaseChainParams
{
public:
    CBaseMainParams()
    {
        nRPCPort = 8332;
    }
};

/**
 * Testnet (v3)
 */
class CBaseTestNetParams : public CBaseChainParams
{
public:
    CBaseTestNetParams()
    {
        nRPCPort = 18332;
        strDataDir = "testnet3";
    }
};

/*
 * Regression test
 */
class CBaseRegTestParams : public CBaseChainParams
{
public:
    CBaseRegTestParams()
    {
        nRPCPort = 18332;
        strDataDir = "regtest";
    }
};

static std::unique_ptr<CBaseChainParams> globalChainBaseParams;

const CBaseChainParams& BaseParams()
{
    assert(globalChainBaseParams);
    return *globalChainBaseParams;
}

std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const std::string& chain)
{
    if (chain == NETWORK_MAIN)
        return std::unique_ptr<CBaseChainParams>(new CBaseMainParams());
    else if (chain == NETWORK_TESTNET)
        return std::unique_ptr<CBaseChainParams>(new CBaseTestNetParams());
    else if (chain == NETWORK_REGTEST)
        return std::unique_ptr<CBaseChainParams>(new CBaseRegTestParams());
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectBaseParams(NetworkType chain)
{
    globalChainBaseParams = CreateBaseChainParams(chain);
}

NetworkType ChainNameFromCommandLine()
{
    bool fRegTest = gArgs.GetBoolArg("-regtest", false);
    bool fTestNet = gArgs.GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
        throw std::runtime_error("Invalid combination of -regtest and -testnet.");
    if (fRegTest)
        return NETWORK_REGTEST;
    if (fTestNet)
        return NETWORK_TESTNET;
    return NETWORK_MAIN;
}
