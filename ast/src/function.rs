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

use crate::node::{IdentifierNode, ListSeparatorNode, Node, OperationNode, SpacingNode};

/// Node that denotes the start of a scope of a function whose declaration is confined to a single
/// line.
#[derive(Debug)]
struct SinglyLinedFunctionScopeDelimiterNode {
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl Node for SinglyLinedFunctionScopeDelimiterNode {
  fn pattern() -> Regex {
    Regex::new(r":").unwrap()
  }

  fn column(&self) -> u32 {}

  fn next(&self) -> &Vec<Option<SpacingNode<OperationNode<impl Node>>>> {
    &vec![Some(SpacingNode {
      next: vec![Some(OperationNode { next: vec![None] })]
    })]
  }
}

/// Node that denotes the end of a declaration of a list of value parameters, started by a
/// [ValueParametersListDeclarationStartNode].
#[derive(Debug)]
struct ValueParametersListDeclarationEndNode {}

impl Node for ValueParametersListDeclarationEndNode {
  fn max_leafing() -> bool {
    false
  }

  fn pattern() -> Regex {
    Regex::new(r"\)").unwrap()
  }

  fn next(&self) -> &Vec<Option<impl Node>> {
    &vec![
      None,
      Some(vec![Some(SinglyLinedFunctionScopeDelimiterNode {})]),
    ]
  }
}

/// Node that denotes the start of a declaration of a list of value parameters. After it, the
/// declaration is expected to be succeeded by the comma-separated value parameters themselves and
/// ended with a closing parenthesis.
#[derive(Debug)]
struct ValueParameterListDeclarationStartNode {}

impl Node for ValueParameterListDeclarationStartNode {
  fn max_leafing() -> bool {
    false
  }

  fn pattern() -> Regex {
    Regex::new(r"\(").unwrap()
  }

  fn next(
    &self
  ) -> &Vec<Option<IdentifierNode<SpacingNode<IdentifierNode<ListSeparatorNode<impl Node>>>>>> {
    &vec![Some(IdentifierNode {
      next: vec![Some(SpacingNode {
        next: vec![Some(IdentifierNode {
          next: vec![
            None,
            Some(ListSeparatorNode {
              next: (*self.next()).clone()
            }),
          ]
        })]
      })]
    })]
  }
}

#[derive(Debug)]
pub(crate) struct ValueParameter<'a> {
  /// Name of the type of the value as it was input by the user: may or may not be qualified.
  pub(crate) type_name: &'a str,

  /// Name attributed to this value parameter.
  pub(crate) identifier: &'a str
}

/// Node of a declaration of a function.
#[derive(Debug)]
pub(crate) struct FunctionNode<'a> {
  /// Identifier of the function.
  pub(crate) name: &'a str,

  /// Values expected to be passed in to the function as parameters.
  pub(crate) valueParameters: Vec<ValueParameter<'a>>,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl<'a> Node for FunctionNode {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn next(&self) -> Vec<Option<IdentifierNode<ValueParameterListDeclarationStartNode>>> {
    vec![Some(IdentifierNode::new(
      self.name,
      self.column,
      self.row,
      vec![Some(ValueParameterListDeclarationStartNode {})]
    ))]
  }
}
