#include <Havoc.h>

auto HavocClient::eventDispatch(
    const json& event
) -> void {
    auto type = std::string();
    auto data = json();

    spdlog::debug( "event: {}", event.dump() );

    if ( ! event.contains( "type" ) ) {
        spdlog::debug( "invalid event: {}", event.dump() );
        return;
    }

    if ( ! event.contains( "data" ) ) {
        spdlog::debug( "invalid event: {}", event.dump() );
        return;
    }

    type = event[ "type" ].get<std::string>();
    data = event[ "data" ].get<json>();

    if ( type == Event::user::login )
    {
        if ( data.empty() ) {
            spdlog::error( "user::login: invalid package (data emtpy)" );
            return;
        }

        spdlog::debug( "user login: {}", data[ "username" ].get<std::string>() );
    }
    else if ( type == Event::user::logout )
    {
        if ( data.empty() ) {
            spdlog::error( "user::logout: invalid package (data emtpy)" );
            return;
        }

        spdlog::debug( "user logout: {}", data[ "username" ].get<std::string>() );
    }
    else if ( type == Event::user::message )
    {

    }
    else if ( type == Event::listener::add )
    {
        if ( data.empty() ) {
            spdlog::error( "listener::register: invalid package (data emtpy)" );
            return;
        }

        MainWindows->PageListener->Protocols.push_back(
            data
        );
    }
    else if ( type == Event::listener::start )
    {

    }
    else if ( type == Event::listener::edit )
    {

    }
    else if ( type == Event::listener::stop )
    {

    }
    else if ( type == Event::listener::status )
    {

    }
    else if ( type == Event::agent::add )
    {

    }
    else if ( type == Event::agent::initialize )
    {

    }
    else if ( type == Event::agent::callback )
    {

    }
    else if ( type == Event::agent::console )
    {

    }
    else if ( type == Event::agent::input )
    {

    }
    else if ( type == Event::agent::status )
    {

    }
    else if ( type == Event::agent::remove )
    {

    } else {
        spdlog::debug( "invalid event: {} not found", type );
    }

}