/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2012 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "LoginDatabase.h"

void LoginDatabaseConnection::DoPrepareStatements()
{
    if(!m_reconnecting)
        m_stmts.resize(MAX_LOGINDATABASE_STATEMENTS);

    PREPARE_STATEMENT(LOGIN_GET_REALMLIST, "SELECT id, name, address, port, icon, color, timezone, allowedSecurityLevel, population, gamebuild FROM realmlist WHERE color <> 3 ORDER BY name", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_EXPIREDIPBANS, "DELETE FROM ip_banned WHERE unbandate<>bandate AND unbandate<=UNIX_TIMESTAMP()", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SET_EXPIREDACCBANS, "UPDATE account_banned SET active = 0 WHERE active = 1 AND unbandate<>bandate AND unbandate<=UNIX_TIMESTAMP()", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_IPBANNED, "SELECT * FROM ip_banned WHERE ip = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_IPAUTOBANNED, "INSERT INTO ip_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, 'Trinity realmd', 'Failed login autoban')", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_ACCBANNED, "SELECT bandate, unbandate FROM account_banned WHERE id = ? AND active = 1", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_ACCAUTOBANNED, "INSERT INTO account_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, 'Trinity realmd', 'Failed login autoban', 1)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_SESSIONKEY, "SELECT a.sessionkey, a.id, aa.gmlevel  FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_VS, "UPDATE account SET v = ?, s = ? WHERE username = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SET_LOGONPROOF, "UPDATE account SET sessionkey = ?, last_ip = ?, last_login = NOW(), locale = ?, os = ?, failed_logins = 0 WHERE username = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_LOGONCHALLENGE, "SELECT a.sha_pass_hash, a.id, a.locked, a.last_ip, aa.gmlevel, a.v, a.s FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE a.username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_FAILEDLOGINS, "UPDATE account SET failed_logins = failed_logins + 1 WHERE username = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_FAILEDLOGINS, "SELECT id, failed_logins FROM account WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_GET_ACCIDBYNAME, "SELECT id FROM account WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_GET_NUMCHARSONREALM, "SELECT numchars FROM realmcharacters WHERE realmid = ? AND acctid= ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_GET_ACCOUNT_BY_IP, "SELECT id FROM account WHERE last_ip = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SET_IP_BANNED, "INSERT INTO ip_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, ?, ?)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SET_IP_NOT_BANNED, "DELETE FROM ip_banned WHERE ip = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SET_ACCOUNT_BANNED, "INSERT INTO account_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, ?, ?, 1)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SET_ACCOUNT_NOT_BANNED, "UPDATE account_banned SET active = 0 WHERE id = ? AND active != 0", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_DEL_REALMCHARACTERS, "DELETE FROM realmcharacters WHERE acctid = ? AND realmid = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_ADD_REALMCHARACTERS, "INSERT INTO realmcharacters (numchars, acctid, realmid) VALUES (?, ?, ?)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_GET_SUM_REALMCHARS, "SELECT SUM(numchars) FROM realmcharacters WHERE acctid = ?", CONNECTION_ASYNC);
}
