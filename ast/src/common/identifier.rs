// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use regex::Regex;

use crate::node::{Node, UnmatchedPatternError};

impl<'a> Node<'a> {
  /// Denotes that an identifier node is expected to follow the current node.
  pub(crate) fn expect_identifier<F: Fn(Self) -> Result<Self, UnmatchedPatternError>>(
    self,
    text: &'a str,
    chain: F
  ) -> Result<Self, UnmatchedPatternError> {
    self.expect_with_pattern(
      Regex::new(r"[a-zA-Z0-9]*").unwrap(),
      || {
        if text.is_empty() {
          return String::from("Expected an identifier.");
        }
        format!(
          "{} is invalid. An identifier can only letters A–Z and digits.",
          text
        )
      },
      text,
      chain
    )
  }
}

#[cfg(test)]
mod tests {
  use crate::node::Node;

  #[test]
  fn errors_when_identifier_is_expected_and_is_empty() {
    println!(
      "{}",
      Node::new("", 0, 0).expect_identifier("", |node| Ok(node)).unwrap()
    );
    assert!(
      Node::new("", 0, 0).expect_identifier("", |node| Ok(node)).ok()
        == Node::new("", 0, 0).expect_identifier("", |node| Ok(node)).ok()
    )
  }
}
