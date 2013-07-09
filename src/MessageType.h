#define stringify ( name ) # name

enum MessageType
{
	NullMessage = 0,
	InitMessage,
	PingMessage,
	KickMessage,
	ClientID,
	WorldInit,
	CreatureInit,
	PlayerJoinLeft,
	PlayerMove,
	ProjectileInit,
	EntityMove,
	EntityUpdate,
	BlockPlace,
	BlockMetadataChange
};