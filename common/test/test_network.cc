/*
Copyright 2026 SAE Institute Switzerland SA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors: Elias Farhan
*/

#ifdef ENABLE_NETWORK

#include <ostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "network/network_manager.h"

namespace ExitGames::Common {
void PrintTo(const JString& str, std::ostream* os) {
  const wchar_t* wide = str.cstr();
  while (*wide) {
    *os << static_cast<char>(*wide++);
  }
}
}  // namespace ExitGames::Common

class MockClient : public common::ClientInterface {
 public:
  MOCK_METHOD(void, OnDebugReturn,
              (int debug_level, const ExitGames::Common::JString& string),
              (override));
  MOCK_METHOD(void, OnConnectionError, (int error_code), (override));
  MOCK_METHOD(void, OnClientError, (int error_code), (override));
  MOCK_METHOD(void, OnWarning, (int warning_code), (override));
  MOCK_METHOD(void, OnServerError, (int error_code), (override));
  MOCK_METHOD(void, OnJoinRoomEvent,
              (int player_nr, const ExitGames::Common::JVector<int>& player_nrs,
               const ExitGames::LoadBalancing::Player& player),
              (override));
  MOCK_METHOD(void, OnLeaveRoomEvent, (int player_nr, bool is_inactive),
              (override));
  MOCK_METHOD(void, OnCustomEvent,
              (int player_nr, nByte event_code,
               const ExitGames::Common::Object& event_content),
              (override));
  MOCK_METHOD(void, OnConnect,
              (int error_code, const ExitGames::Common::JString& error_string,
               const ExitGames::Common::JString& region,
               const ExitGames::Common::JString& cluster),
              (override));
  MOCK_METHOD(void, OnDisconnect, (), (override));
  MOCK_METHOD(void, OnLeaveRoom,
              (int error_code, const ExitGames::Common::JString& error_string),
              (override));
};

TEST(Network, Connect) {
  MockClient client;
  common::NetworkManager network_manager(&client);
  network_manager.Begin();
  network_manager.FixedUpdate();
  network_manager.End();
}

#endif
