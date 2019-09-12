open Js;
open Expo;
open ReactNative;
open ReactNative.Style;
open ReasonDateFns;

let authenticationOptions =
  LocalAuthentication.authenticateAsyncOptions(
    ~promptMessage="Please authorize in the application before you start",
    (),
  );

let styles =
  StyleSheet.create({
    "notAuthorizedContainer":
      style(
        ~flex=1.,
        ~justifyContent=`center,
        ~alignItems=`center,
        ~paddingTop=32.->dp,
        ~paddingBottom=32.->dp,
        ~backgroundColor="#F5FCFF",
        (),
      ),
  });

type authenticationState =
  | Success
  | NotAuthorized
  | NotAvailable
  | InBackground(Js.Date.t);

let window = Dimensions.get(`window); 

[@react.component]
let make = (~children) => {
  let (authState, setAuthenticated) = React.useState(() => NotAuthorized);

  let authenticate = _ => {
    LocalAuthentication.authenticateAsync(authenticationOptions)
    |> Promise.then_(authResult =>
         setAuthenticated(_ =>
           LocalAuthentication.successGet(authResult)
             ? Success : NotAuthorized
         )
         |> Promise.resolve
       )
    |> Promise.catch(_ =>
         setAuthenticated(_ => NotAvailable) |> Promise.resolve
       )
    |> ignore;
  };

  React.useEffect0(() => {
    if (authState !== Success) {
      authenticate();
    };

    None;
  });

  let handleAppStateChange = nextAppState => {
    Js.log(nextAppState);

    if (nextAppState === AppState.background
        || nextAppState === AppState.inactive) {
      setAuthenticated(_ => InBackground(Js.Date.make()));
    } else if (nextAppState === AppState.active) {
      let newState =
        switch (authState) {
        | InBackground(date) =>
          DateFns.differenceInMinutes(Js.Date.make(), date) < 5.
            ? Success : NotAuthorized
        | state => state
        };

      if (authState !== newState) {
        setAuthenticated(_ => newState);
      };
    };
  };

  React.useEffect(() => {
    AppState.addEventListener(`change(state => handleAppStateChange(state)))
    |> ignore;

    Some(() => AppState.removeEventListener(`change(handleAppStateChange)));
  });

  switch (authState) {
  | Success => children
  | InBackground(_) =>
    <>
      <View style=StyleSheet.absoluteFill> ...children </View>
      <BlurView style=StyleSheet.absoluteFill tint=`light intensity=99. />
    </>
  | NotAvailable =>
    <View style=styles##notAuthorizedContainer>
      <Text>
        {React.string(
           "Please enable your local device authentication (TouchID or FaceID)",
         )}
      </Text>
    </View>
  | NotAuthorized =>
    <View style=styles##notAuthorizedContainer>
      <StyledText variant=LargeTitle i18n="Unlock" />
      <StyledText variant=Title i18n="Unlock device to use this application" />
      <View
        style={style(
          ~position=`relative,
          ~width=(window##width *. 0.6)->dp,
          ~flex=1.,
          (),
        )}>
        <Lottie
          autoPlay=true
          loop=false
          source={Packager.require(
            "../../../assets/animations/faceId.json",
          )}
        />
      </View>
      <LargeButton i18n="Unlock" onPress=authenticate />
    </View>
  };
};