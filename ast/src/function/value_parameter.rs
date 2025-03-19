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

use std::sync::OnceLock;
use regex::Regex;

use crate::{
  branch::Branch,
  common::{identifier::IdentifierNode, spacing::SpacingNode},
  node::{matching, ListSeparatorNode, Node, UnmatchedPatternError}
};

/// Node that denotes the end of a declaration of a list of value parameters, started by a
/// [ValueParametersListDeclarationStartNode].
#[derive(Debug)]
struct ValueParametersListDeclarationEndNode {
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl Node for ValueParametersListDeclarationEndNode {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, UnmatchedPatternError> {
    Ok(")")
  }

  fn branch(&self) -> &Vec<Option<impl Node>> {
    &vec![
      None,
      Some(vec![Some(
        crate::function::function::SinglyLinedFunctionScopeDelimiterNode {}
      )]),
    ]
  }
}

/// Node that denotes the start of a declaration of a list of value parameters. After it, the
/// declaration is expected to be succeeded by the comma-separated value parameters themselves and
/// ended with a closing parenthesis.
#[derive(Debug)]
struct ValueParameterListDeclarationStartNode {}

impl<'a> Node for ValueParameterListDeclarationStartNode {
  fn text(&self) -> Result<&'a str, UnmatchedPatternError> {
    Ok(")")
  }

  fn branch(
    &self
  ) -> &Branch<
    IdentifierNode<'a, SpacingNode<'a, IdentifierNode<'a, ListSeparatorNode<'a, impl Node>>>>
  > {
    static BRANCH: OnceLock<Branch<'a, ValueParameterListDeclarationStartNode, SpacingNode<'a, IdentifierNode<'a, >>>>
    &vec![Some(IdentifierNode {
      expected_next: Vec![Some(SpacingNode {
        expected_next: Vec![Some(IdentifierNode {
          expected_next: Vec![
            None,
            Some(ListSeparatorNode {
              next: (*self.expected_next()).clone()
            }),
          ]
        })]
      })]
    })]
  }
}
