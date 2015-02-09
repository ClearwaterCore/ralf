/**
 * @file test_sessionstore.cpp UT for Ralf session store.
 *
 * Project Clearwater - IMS in the Cloud
 * Copyright (C) 2013  Metaswitch Networks Ltd
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version, along with the "Special Exception" for use of
 * the program along with SSL, set forth below. This program is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * The author can be reached by email at clearwater@metaswitch.com or by
 * post at Metaswitch Networks Ltd, 100 Church St, Enfield EN2 6BQ, UK
 *
 * Special Exception
 * Metaswitch Networks Ltd  grants you permission to copy, modify,
 * propagate, and distribute a work formed by combining OpenSSL with The
 * Software, or a work derivative of such a combination, even if such
 * copying, modification, propagation, or distribution would otherwise
 * violate the terms of the GPL. You must comply with the GPL in all
 * respects for all of the code used other than OpenSSL.
 * "OpenSSL" means OpenSSL toolkit software distributed by the OpenSSL
 * Project and licensed under the OpenSSL Licenses, or a work based on such
 * software and licensed under the OpenSSL Licenses.
 * "OpenSSL Licenses" means the OpenSSL License and Original SSLeay License
 * under which the OpenSSL Project distributes the OpenSSL toolkit software,
 * as those licenses appear in the file LICENSE-OPENSSL.
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "localstore.h"
#include "sessionstore.h"

static const SAS::TrailId FAKE_TRAIL = 0;

// These tests use "typed tests" to run the same tests over different
// (de)serializers. For more information see:
// https://code.google.com/p/googletest/wiki/AdvancedGuide#Typed_Tests

/// The types of (de)serializer that we want to test.
typedef ::testing::Types<
  SessionStore::BinarySerializerDeserializer,
  SessionStore::JsonSerializerDeserializer
> SerializerDeserializerTypes;

/// Fixture for BasicSessionStoreTest.  This uses a single SessionStore,
/// configured to use exactly one (de)serializer.
///
/// The fixture is a template, parameterized over the different types of
/// (de)serializer.
template<class T>
class BasicSessionStoreTest : public ::testing::Test
{
  BasicSessionStoreTest()
  {
    _memstore = new LocalStore();

    SessionStore::SerializerDeserializer* serializer = new T();
    std::vector<SessionStore::SerializerDeserializer*> deserializers = {
      new T()
    };

    _store = new SessionStore(_memstore, serializer, deserializers);
  }

  virtual ~BasicSessionStoreTest()
  {
    delete _store; _store = NULL;
    delete _memstore; _memstore = NULL;
  }

  LocalStore* _memstore;
  SessionStore* _store;
};

// BasicSessionStoreTest is parameterized over these types.
TYPED_TEST_CASE(BasicSessionStoreTest, SerializerDeserializerTypes);

TYPED_TEST(BasicSessionStoreTest, SimpleTest)
{
  SessionStore::Session* session = new SessionStore::Session();
  session->session_id = "session_id";
  session->ccf.push_back("ccf1");
  session->ccf.push_back("ccf2");
  session->acct_record_number = 2;
  session->timer_id = "timer_id";
  session->session_refresh_time = 5 * 60;

  // Save the session in the store
  bool rc = this->_store->set_session_data("call_id", ORIGINATING, SCSCF, session, FAKE_TRAIL);
  EXPECT_EQ(true, rc);
  delete session; session = NULL;

  // Retrieve the session again.
  session = this->_store->get_session_data("call_id", ORIGINATING, SCSCF, FAKE_TRAIL);
  ASSERT_TRUE(session != NULL);
  EXPECT_EQ("session_id", session->session_id);
  EXPECT_EQ(2u, session->acct_record_number);
  EXPECT_EQ("timer_id", session->timer_id);
  EXPECT_EQ(5u * 60, session->session_refresh_time);
  EXPECT_EQ(2u, session->ccf.size());

  delete session; session = NULL;
}

TYPED_TEST(BasicSessionStoreTest, DeletionTest)
{
  SessionStore::Session* session = new SessionStore::Session();
  session->session_id = "session_id";
  session->ccf.push_back("ccf1");
  session->ccf.push_back("ccf2");
  session->acct_record_number = 2;
  session->timer_id = "timer_id";
  session->session_refresh_time = 5 * 60;

  // Save the session in the store
  bool rc = this->_store->set_session_data("call_id", ORIGINATING, SCSCF, session, FAKE_TRAIL);
  EXPECT_EQ(true, rc);
  delete session; session = NULL;

  this->_store->delete_session_data("call_id", ORIGINATING, SCSCF, FAKE_TRAIL);

  // Retrieve the session again.
  session = this->_store->get_session_data("call_id", ORIGINATING, SCSCF, FAKE_TRAIL);
  EXPECT_EQ(NULL, session);

  delete session; session = NULL;
}
