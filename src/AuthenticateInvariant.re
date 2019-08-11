open Js;
open Expo;
open ReactNative;
open ReactNative.Style;

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
        ~backgroundColor="#F5FCFF",
        (),
      ),
  });

type authenticationState =
  | Success
  | NotAuthorized
  | NotAvailable;
  
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
    authenticate();
    None;
  });

  switch (authState) {
  | Success => children
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
      <Text> {React.string("Please authorize before statrt")} </Text>
      <Button title="authenticate" onPress=authenticate />
    </View>
  };
};