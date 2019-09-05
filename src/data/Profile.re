open Db;

type profileContextValue = {
  data: option(profile),
  updateProfile: profile => unit,
};

let profileContext = React.createContext((None, ignore));
let useProfile = () => React.useContext(profileContext);
let useProfileOrFail = () => {
  let (profile, updateProfile) = useProfile();

  switch (profile) {
  | Some(profile) => (profile, updateProfile)
  | None => raise(Not_found)
  };
};

module ProfileProvider = {
  let makeProps = Helpers.makeProviderProps;
  let make = React.Context.provider(profileContext);
};

[@react.component]
let make = (~children) => {
  let (profile, setProfile) = React.useState(_ => None);

  React.useEffect0(() => {
    Db.getProfile(profile =>
      setProfile(_ =>
        switch (profile) {
        | None => None
        | Some(profileObj) =>
          Some({
            name: profileObj |> Db.nameGet,
            goal: profileObj |> Db.goalGet,
            initialWeight: profileObj |> Db.initialWeightGet,
          })
        }
      )
    );

    None;
  });

  let updateProfile = data => {
    (data.name, data.goal, data.initialWeight)
    |> Db.insertProfile(_ => setProfile(_ => Some(data)));
  };

  <ProfileProvider value=(profile, updateProfile)>
    ...children
  </ProfileProvider>;
};